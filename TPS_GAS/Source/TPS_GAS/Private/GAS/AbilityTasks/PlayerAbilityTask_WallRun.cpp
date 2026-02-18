#include "GAS/AbilityTasks/PlayerAbilityTask_WallRun.h"
#include <GameFramework\CharacterMovementComponent.h>
#include <TPS_GAS/TPS_GASCharacter.h>
#include <Components/CapsuleComponent.h>
#include <Kismet/KismetSystemLibrary.h>


UPlayerAbilityTask_WallRun* UPlayerAbilityTask_WallRun::CreateWallRunTask(UGameplayAbility* OwningAbility, ACharacter* InCharacterOwner, UCharacterMovementComponent* InCharacterMovement, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes)
{
	UPlayerAbilityTask_WallRun* WallRunTask = NewAbilityTask<UPlayerAbilityTask_WallRun>(OwningAbility);

	WallRunTask->CharacterMovement = InCharacterMovement;
	WallRunTask->CharacterOwner = InCharacterOwner;
	WallRunTask->bTickingTask = true;
	WallRunTask->WallRun_TraceObjectTypes = TraceObjectTypes;

	return WallRunTask;
}

void UPlayerAbilityTask_WallRun::Activate()
{
	Super::Activate();

	FHitResult OnWallHit;
	//const FVector CurrentAcceleration = CharacterMovement->GetCurrentAcceleration();

	if (!FindRunnableWall(OnWallHit)) 
	{
		//found the wall we want to run on
		if (ShouldBroadcastAbilityTaskDelegates()) //makes sure ability is actually active and then lets you call delegates
		{
			OnWallRunFinished.Broadcast();
		}

		EndTask();
		return;
	}

	OnWallFound.Broadcast(IsWallOnTheLeft(OnWallHit));

	CharacterOwner->Landed(OnWallHit);

	CharacterOwner->SetActorLocation(OnWallHit.ImpactPoint + OnWallHit.ImpactNormal * 80.0f);

	CharacterMovement->SetMovementMode(EMovementMode::MOVE_Flying);
}

void UPlayerAbilityTask_WallRun::OnDestroy(bool bInOwnerFinished)
{
	CharacterMovement->SetPlaneConstraintEnabled(false);
	CharacterMovement->SetMovementMode(EMovementMode::MOVE_Falling);

	Super::OnDestroy(bInOwnerFinished);
}


void UPlayerAbilityTask_WallRun::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	FHitResult OnWallHit;

	if (!FindRunnableWall(OnWallHit))
	{
		//found the wall we want to run on
		if (ShouldBroadcastAbilityTaskDelegates()) //makes sure ability is actually active and then lets you call delegates
		{
			OnWallRunFinished.Broadcast();
		}

		EndTask();
		return;
	}

	const FRotator DirectionToRotate = IsWallOnTheLeft(OnWallHit) ? FRotator(0.0f, -90.0f, 0.0f) : FRotator(0.0f, 90.0f, 0.0f); //flip to wall direction
	const FVector WallRunDirection = DirectionToRotate.RotateVector(OnWallHit.ImpactNormal); //where are we running mate?

	CharacterMovement->Velocity = WallRunDirection * 700.0f; //TODO: Vinay make this a variable
	CharacterMovement->Velocity.Z = 0.0f; //CharacterMovement->GetGravityZ() * DeltaTime;

	CharacterOwner->SetActorRotation(WallRunDirection.Rotation());

	//set constraints
	CharacterMovement->SetPlaneConstraintEnabled(true);
	CharacterMovement->SetPlaneConstraintOrigin(OnWallHit.ImpactPoint);
	CharacterMovement->SetPlaneConstraintNormal(OnWallHit.ImpactNormal);
}

bool UPlayerAbilityTask_WallRun::FindRunnableWall(FHitResult& OnWallHit)
{

	const FVector CurrentCharacterLocation = CharacterOwner->GetActorLocation();
	const FVector RightVector = CharacterOwner->GetActorRightVector();
	const FVector ForwardVector = CharacterOwner->GetActorForwardVector();

	const float TraceLength = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() + 30.0f;
	TArray<AActor*> ActorsToIgnore = { CharacterOwner };

	FHitResult TraceHit;
	
	static auto const CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowDebugTraversal"));
	const bool bShowTraversal = CVar->GetInt() > 0;
	EDrawDebugTrace::Type DrawType = bShowTraversal ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None; //debug

	//forward trace
	if (UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(), 
		CurrentCharacterLocation, 
		CurrentCharacterLocation + ForwardVector * TraceLength,
		WallRun_TraceObjectTypes,
		true,
		ActorsToIgnore,
		DrawType,
		OnWallHit,
		true))
	{
		//found a wall in front, can't wall run there mate
		return false;
	}
	
	//left trace
	if (UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(), 
		CurrentCharacterLocation, 
		CurrentCharacterLocation + -RightVector * TraceLength,
		WallRun_TraceObjectTypes,
		true,
		ActorsToIgnore,
		DrawType,
		OnWallHit,
		true))
	{
		if (FVector::DotProduct(OnWallHit.ImpactNormal, RightVector) > 0.3f) //check with right vector
		{
			return true;
		}
	}
	
	
	//right trace
	if (UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(), 
		CurrentCharacterLocation, 
		CurrentCharacterLocation + RightVector * TraceLength,
		WallRun_TraceObjectTypes,
		true,
		ActorsToIgnore,
		DrawType,
		OnWallHit,
		true))
	{
		if (FVector::DotProduct(OnWallHit.ImpactNormal, -RightVector) > 0.3f) //check with left vector
		{
			return true;
		}
	}

	return false;
}

bool UPlayerAbilityTask_WallRun::IsWallOnTheLeft(const FHitResult& InWallHit) const
{
	return FVector::DotProduct(CharacterOwner->GetActorRightVector(), InWallHit.ImpactNormal) > 0.0f;
}

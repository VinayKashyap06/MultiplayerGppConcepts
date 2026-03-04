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

	if (!FindRunnableWall(OnWallHit))
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnWallRunFinished.Broadcast();
		}

		EndTask();
		return;
	}

	OnWallFound.Broadcast(IsWallOnTheLeft(OnWallHit));

	CharacterMovement->SetMovementMode(MOVE_Falling);

	// Set constraint ONCE
	CharacterMovement->SetPlaneConstraintEnabled(true);
	CharacterMovement->SetPlaneConstraintOrigin(OnWallHit.ImpactPoint);
	CharacterMovement->SetPlaneConstraintNormal(OnWallHit.ImpactNormal);

	// Reduce gravity instead of disabling it completely
	CharacterMovement->GravityScale = 0.4f;
}


void UPlayerAbilityTask_WallRun::OnDestroy(bool bInOwnerFinished)
{

	CharacterMovement->SetPlaneConstraintEnabled(false);

	CharacterMovement->GravityScale = 1.0f;

	CharacterMovement->SetMovementMode(MOVE_Falling);

	Super::OnDestroy(bInOwnerFinished);
}


void UPlayerAbilityTask_WallRun::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	FHitResult OnWallHit;

	if (!FindRunnableWall(OnWallHit))
	{
		//no wall found we want to run on
		if (ShouldBroadcastAbilityTaskDelegates()) //makes sure ability is actually active and then lets you call delegates
		{
			OnWallRunFinished.Broadcast();
		}

		UE_LOG(LogTemp, Warning, TEXT("ENDING TASK from tick"));
		EndTask();
		return;
	}

	const FRotator DirectionToRotate =
		IsWallOnTheLeft(OnWallHit)
		? FRotator(0.f, -90.f, 0.f)
		: FRotator(0.f, 90.f, 0.f);

	const FVector WallRunDirection =
		DirectionToRotate.RotateVector(OnWallHit.ImpactNormal);

	// Preserve existing Z slightly for smoother motion
	FVector NewVelocity = WallRunDirection * 500.0f;
	NewVelocity.Z = 0.0f;

	CharacterMovement->Velocity = FMath::VInterpTo(
		CharacterMovement->Velocity,
		NewVelocity,
		DeltaTime,
		10.0f
	);

	CharacterOwner->SetActorRotation(WallRunDirection.Rotation());

}

bool UPlayerAbilityTask_WallRun::FindRunnableWall(FHitResult& OnWallHit)
{

	const FVector CurrentCharacterLocation = CharacterOwner->GetActorLocation();
	const FVector RightVector = CharacterOwner->GetActorRightVector();
	const FVector ForwardVector = CharacterOwner->GetActorForwardVector();

	const float TraceLength = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() + 40.0f;
	TArray<AActor*> ActorsToIgnore = { CharacterOwner };

	FHitResult TraceHit;
	
	static auto const CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowDebugTraversal"));
	const bool bShowTraversal = CVar->GetInt() > 0;
	EDrawDebugTrace::Type DrawType = bShowTraversal ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None; //debug


	float VerticalDot =
		FVector::DotProduct(OnWallHit.ImpactNormal, FVector::UpVector);

	// Reject floors & ceilings
	if (FMath::Abs(VerticalDot) > 0.3f)
	{
		return false;
	}


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

		UE_LOG(LogTemp, Warning, TEXT("Wall in front"));
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
		if (FVector::DotProduct(OnWallHit.ImpactNormal, RightVector) > 0.5f) //check with right vector
		{
			UE_LOG(LogTemp, Warning, TEXT("Wall on left"));
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
		if (FVector::DotProduct(OnWallHit.ImpactNormal, -RightVector) > 0.5f) //check with left vector
		{
			UE_LOG(LogTemp, Warning, TEXT("Wall on right"));
			return true;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("No Wall"));
	return false;
}

bool UPlayerAbilityTask_WallRun::IsWallOnTheLeft(const FHitResult& InWallHit) const
{
	return FVector::DotProduct(CharacterOwner->GetActorRightVector(), InWallHit.ImpactNormal) > 0.0f;
}

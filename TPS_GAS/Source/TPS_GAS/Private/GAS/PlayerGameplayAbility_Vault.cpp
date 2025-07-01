#include "GAS/PlayerGameplayAbility_Vault.h"
#include "TPS_GAS/TPS_GASCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Components/CapsuleComponent.h"
#include "Player/PlayerMovementComponent.h"
#include "Player/PlayerMotionWarpingComponent.h"


UPlayerGameplayAbility_Vault::UPlayerGameplayAbility_Vault()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UPlayerGameplayAbility_Vault::CommitCheck(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags)
{
	if (!Super::CommitCheck(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags))
		return false;

	ATPS_GASCharacter* Character = GetPlayerCharacterFromActorInfo();
	if (!IsValid(Character))
	{
		return false;
	}

	const FVector StartLocation = Character->GetActorLocation();
	const FVector ForwardVector = Character->GetActorForwardVector();
	const FVector UpVector = Character->GetActorUpVector();

	TArray<AActor*> ActorsToIgnore = { Character }; //ignore self 
	static auto const CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowDebugTraversal"));
	const bool bShowTraversal = CVar->GetInt() > 0;

	EDrawDebugTrace::Type DrawType = bShowTraversal ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	bool bJumpToLocation = false;

	int32 JumpToLocationIndex = INDEX_NONE;
	int i = 0;
	FHitResult TraceHitResult;
	float MaxJumpDistance = HorizontalTraceLength;

	//HorizontalTrace first
	for (; i < HorizontalTraceCount; i++)
	{
		const FVector TraceStart = StartLocation + i * UpVector * HorizontalTraceStep;
		const FVector TraceEnd = TraceStart + ForwardVector * HorizontalTraceLength;

		//sphere trace to not miss complex geometry or even small objects
		if (UKismetSystemLibrary::SphereTraceSingleForObjects(this, TraceStart, TraceEnd, HorizontalTraceRadius, TraceObjectTypes, true, ActorsToIgnore, DrawType, TraceHitResult, true))
		{
			//if our initial jump location is null/ then we say, let's jump to that
			if (JumpToLocationIndex == INDEX_NONE && i < HorizontalTraceCount - 1)
			{
				JumpToLocationIndex = i;
				JumpToLocation = TraceHitResult.Location;
			}
			else if (JumpToLocationIndex == i - 1) //we already had a jump location and we found an obstacle
			{
				MaxJumpDistance = FVector::Dist2D(TraceHitResult.Location, TraceStart);
				break;
			}
		}
		else if (JumpToLocationIndex != INDEX_NONE)
		{
			break; //we already have a jump location we want our character to jump to, break out of loop and jump to target
		}
	}

	if (JumpToLocationIndex == INDEX_NONE)
	{
		return false; //we don't need to vault to anything
	}

	const float DistanceToJumpTo = FVector::Dist2D(StartLocation, JumpToLocation);
	const float MaxVerticalTraceDistance = MaxJumpDistance - DistanceToJumpTo;

	if (MaxVerticalTraceDistance < 0)
	{
		return false;
	}

	if (i == HorizontalTraceCount)
	{
		i--;
	}

	//how far we can should vertically?
	VerticalTraceLength = FMath::Abs(JumpToLocation.Z - (StartLocation * i * UpVector * HorizontalTraceStep).Z);
	FVector VerticalStartLocation = JumpToLocation + UpVector * VerticalTraceLength;

	i = 0;
	VerticalTraceCount = MaxVerticalTraceDistance / VerticalTraceStep;
	bool bJumpOverLocationSet = false;
	//vertical trace to make sure we have enough space to clearly jump/vault over and nothing will block us
	//after performing jump, we should be able to stand/fall over either on the obstacle or on the other side of obstacle
	for (; i < VerticalTraceCount; i++)
	{
		const FVector TraceStart = VerticalStartLocation + i * ForwardVector * VerticalTraceStep;
		const FVector TraceEnd = TraceStart + UpVector * VerticalTraceLength * -1;

		if (UKismetSystemLibrary::SphereTraceSingleForObjects(this, TraceStart, TraceEnd, VerticalTraceRadius, TraceObjectTypes, true, ActorsToIgnore, DrawType, TraceHitResult, true))
		{
			JumpOverLocation = TraceHitResult.ImpactPoint;

			if (i == 0)
			{
				JumpToLocation = JumpOverLocation; //we hit something overhead on first trace, so we cannot jump over, setting jump to location to whatever we hit
			}
		}
		else if (i != 0) //trace failed and we collided with nothing so we can break out
		{
			bJumpOverLocationSet = true;
			break;
		}
	}

	if (!bJumpOverLocationSet)
	{
		return false;
	}
	const FVector TraceStart = JumpOverLocation + ForwardVector * VerticalTraceStep;

	if (UKismetSystemLibrary::SphereTraceSingleForObjects(this, TraceStart, JumpOverLocation, VerticalTraceRadius, TraceObjectTypes, true, ActorsToIgnore, DrawType, TraceHitResult, true)) {
		JumpOverLocation = TraceHitResult.ImpactPoint;
	}

	if (bShowTraversal)
	{
		DrawDebugSphere(GetWorld(), JumpToLocation, 15.0f, 15.0f, FColor::White, false, 7);
		DrawDebugSphere(GetWorld(), JumpOverLocation, 15.0f, 15.0f, FColor::White, false, 7);
	}

	return true;

}

void UPlayerGameplayAbility_Vault::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//Check if we can commit ability
	FGameplayTagContainer* OptionalRelevantTags = nullptr;
	if (!CommitCheck(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags))
	{
		K2_EndAbility();
		return;
	}

	if (const ATPS_GASCharacter* Character = GetPlayerCharacterFromActorInfo())
	{
		if (UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement())
		{
			CharacterMovement->SetMovementMode(MOVE_Flying); //we have to set movement as flying to consider vault over points, we will set it to falling later
		}

		if (UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent())
		{
			for (ECollisionChannel Channel : CollisionChannelsToIgnore)
			{
				CapsuleComponent->SetCollisionResponseToChannel(Channel, ECollisionResponse::ECR_Ignore); //ignore collisions which aren't required
			}
		}

		//Check the reference
		//https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Plugins/MotionWarping/UMotionWarpingComponent?application_version=5.1
		if (UPlayerMotionWarpingComponent* MotionWarpingComponent = Character->GetMotionWarpingComponent())
		{
			MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("JumpToLocation"), JumpToLocation, Character->GetActorRotation());
			MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("JumpOverLocation"), JumpOverLocation, Character->GetActorRotation());
		}
	}

	//https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Plugins/GameplayAbilities/Abilities/Tasks/UAbilityTask_PlayMontageAndWait
	MontagePlayAwaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, VaultMontage);

	//we end ability if our anim task cannot be played for some reason
	MontagePlayAwaitTask->OnBlendOut.AddDynamic(this, &UPlayerGameplayAbility_Vault::K2_EndAbility);
	MontagePlayAwaitTask->OnCompleted.AddDynamic(this, &UPlayerGameplayAbility_Vault::K2_EndAbility);
	MontagePlayAwaitTask->OnInterrupted.AddDynamic(this, &UPlayerGameplayAbility_Vault::K2_EndAbility);
	MontagePlayAwaitTask->OnCancelled.AddDynamic(this, &UPlayerGameplayAbility_Vault::K2_EndAbility);
	MontagePlayAwaitTask->ReadyForActivation(); //Important for activating this task, which will play the montage
}

void UPlayerGameplayAbility_Vault::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(MontagePlayAwaitTask))
	{
		MontagePlayAwaitTask->EndTask();
	}

	if (const ATPS_GASCharacter* Character = GetPlayerCharacterFromActorInfo())
	{
		if (UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent())
		{
			for (ECollisionChannel Channel : CollisionChannelsToIgnore)
			{
				CapsuleComponent->SetCollisionResponseToChannel(Channel, ECollisionResponse::ECR_Block);
			}
		}

		//set mode to falling
		UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();
		if (CharacterMovement && CharacterMovement->IsFlying())
		{
			CharacterMovement->SetMovementMode(MOVE_Falling);
		}

		//Remove warp targets
		//These warp targets are stored as map inside motion warping component
		if (UPlayerMotionWarpingComponent* MotionWarpingComponent = Character->GetMotionWarpingComponent())
		{
			MotionWarpingComponent->RemoveWarpTarget(TEXT("JumpToLocation"));
			MotionWarpingComponent->RemoveWarpTarget(TEXT("JumpOverLocation"));
		}

	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

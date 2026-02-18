#include "GAS/PlayerGameplayAbility_WallRun.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/Character.h>
#include <Components/CapsuleComponent.h>
#include <TPS_GAS/TPS_GASCharacter.h>
#include <GAS/PlayerAbilitySystemComponent.h>
#include <GAS/AbilityTasks/PlayerAbilityTask_WallRun.h>

UPlayerGameplayAbility_WallRun::UPlayerGameplayAbility_WallRun()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPlayerGameplayAbility_WallRun::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	ACharacter* CharacterAvatar = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (CharacterAvatar)
	{
		UCapsuleComponent* CapsuleCompoennt = CharacterAvatar->GetCapsuleComponent();
		if (CapsuleCompoennt)
		{
			CapsuleCompoennt->OnComponentHit.AddDynamic(this, &ThisClass::OnCapsuleComponentHit);
		}
	}
}

void UPlayerGameplayAbility_WallRun::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (ActorInfo)
	{
		ACharacter* CharacterAvatar = Cast<ACharacter>(ActorInfo->AvatarActor);
		if (CharacterAvatar)
		{
			UCapsuleComponent* CapsuleCompoennt = CharacterAvatar->GetCapsuleComponent();
			if (CapsuleCompoennt)
			{
				CapsuleCompoennt->OnComponentHit.RemoveDynamic(this, &ThisClass::OnCapsuleComponentHit);
			}
		}
	}
	Super::OnRemoveAbility(ActorInfo, Spec);
}

bool UPlayerGameplayAbility_WallRun::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags)
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	ATPS_GASCharacter* characterAvatar = GetPlayerCharacterFromActorInfo();

	//we can only activate it if character is valid and not moving on ground
	return characterAvatar && !characterAvatar->GetCharacterMovement()->IsMovingOnGround();
}

void UPlayerGameplayAbility_WallRun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	WallRunTask = UPlayerAbilityTask_WallRun::CreateWallRunTask(this, Cast<ACharacter>(GetAvatarActorFromActorInfo()), Cast<UCharacterMovementComponent>(ActorInfo->MovementComponent), WallRun_TraceObjectTypes);

	WallRunTask->OnWallRunFinished.AddDynamic(this, &ThisClass::K2_EndAbility);
	WallRunTask->OnWallFound.AddDynamic(this, &ThisClass::OnWallSideFound);
	WallRunTask->ReadyForActivation();
}


void UPlayerGameplayAbility_WallRun::OnCapsuleComponentHit(UPrimitiveComponent* WithComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UAbilitySystemComponent* AbilitySystemComp = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystemComp)
	{
		const FVector Normal = Hit.ImpactNormal;
		bool bIsFloor = true;

		/*if (Hit.PhysMaterial.IsValid())
		{
			EPhysicalSurface Surface =
				UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			bIsFloor = Surface == SurfaceType_Default;
		}*/
		float Dot = FVector::DotProduct(Hit.ImpactNormal, FVector::UpVector);
		bool bIsWall = Dot < 0.3f && Dot > -0.3f; // near vertical

		////if (!bIsFloor)
		//if (bIsWall)
		//{
		//}
			AbilitySystemComp->TryActivateAbility(GetCurrentAbilitySpecHandle());
	}
}


void UPlayerGameplayAbility_WallRun::OnWallSideFound(bool bLeftSide)
{
	ATPS_GASCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	UAbilitySystemComponent* AbilitySystemComp = GetAbilitySystemComponentFromActorInfo();

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComp->MakeEffectContext();

	if (bLeftSide)
	{
		PlayerCharacter->ApplyGameplayEffectToSelf(WallRunLeftSideEffects, EffectContextHandle);
	}
	else
	{
		PlayerCharacter->ApplyGameplayEffectToSelf(WallRunRightSideEffects, EffectContextHandle);
	}
}


void UPlayerGameplayAbility_WallRun::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(WallRunTask))
	{
		WallRunTask->EndTask();
	}

	UAbilitySystemComponent* AbilitySystemComp = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystemComp)
	{
		AbilitySystemComp->RemoveActiveGameplayEffectBySourceEffect(WallRunLeftSideEffects, AbilitySystemComp);
		AbilitySystemComp->RemoveActiveGameplayEffectBySourceEffect(WallRunRightSideEffects, AbilitySystemComp);
	}


	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
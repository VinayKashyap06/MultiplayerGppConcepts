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

	return false;
}

void UPlayerGameplayAbility_WallRun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
}

void UPlayerGameplayAbility_WallRun::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
}

void UPlayerGameplayAbility_WallRun::OnCapsuleComponentHit(UPrimitiveComponent* WithComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{

}

void UPlayerGameplayAbility_WallRun::OnWallSideFound(bool bLeftSide)
{

}
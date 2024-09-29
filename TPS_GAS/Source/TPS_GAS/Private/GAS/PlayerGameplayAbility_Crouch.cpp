// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PlayerGameplayAbility_Crouch.h"
#include "TPS_GAS/TPS_GASCharacter.h"

UPlayerGameplayAbility_Crouch::UPlayerGameplayAbility_Crouch()
{
	//Reference : https://docs.unrealengine.com/4.27/en-US/InteractiveExperiences/GameplayAbilitySystem/GameplayAbility/
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UPlayerGameplayAbility_Crouch::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OUT OptionalRelevantTags))
	{
		return false;
	}

	const ATPS_GASCharacter* Character = CastChecked<ATPS_GASCharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	return Character->CanCrouch();
}

void UPlayerGameplayAbility_Crouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			return;
		}
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ATPS_GASCharacter* Character = CastChecked<ATPS_GASCharacter>(ActorInfo->AvatarActor.Get());
	Character->Crouch();
	Character->SetCrouchedCamera(true);
}


void UPlayerGameplayAbility_Crouch::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ATPS_GASCharacter* Character = CastChecked<ATPS_GASCharacter>(ActorInfo->AvatarActor.Get());
	Character->UnCrouch();
	Character->SetCrouchedCamera(false);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

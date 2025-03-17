// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PlayerGameplayAbility_Vault.h"

UPlayerGameplayAbility_Vault::UPlayerGameplayAbility_Vault()
{
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
    InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

bool UPlayerGameplayAbility_Vault::CommitCheck(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags)
{
    return false;
}

void UPlayerGameplayAbility_Vault::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UPlayerGameplayAbility_Vault::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

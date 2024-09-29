// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PlayerGameplayAbility_Vault.h"

UPlayerGameplayAbility_Vault::UPlayerGameplayAbility_Vault()
{

}

bool UPlayerGameplayAbility_Vault::CommitCheck(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags)
{
    return false;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/PlayerBaseGameplayAbility.h"
#include "PlayerGameplayAbility_Vault.generated.h"

/**
 * 
 */
UCLASS()
class TPS_GAS_API UPlayerGameplayAbility_Vault : public UPlayerBaseGameplayAbility
{
	GENERATED_BODY()
	UPlayerGameplayAbility_Vault();
	
public:
	virtual bool CommitCheck(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags) override;
};

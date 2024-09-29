// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/PlayerBaseGameplayAbility.h"
#include "PlayerGameplayAbility_Jump.generated.h"


class UGameplayEffect;

/**
 * 
 */
UCLASS()
class TPS_GAS_API UPlayerGameplayAbility_Jump : public UPlayerBaseGameplayAbility
{
	GENERATED_BODY()
public:
	UPlayerGameplayAbility_Jump();

protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	//since player lands after ability is activated, we don't need a end ability call, it is called via clearing all IN-AIR tags, Player Character -> OnLanded
};

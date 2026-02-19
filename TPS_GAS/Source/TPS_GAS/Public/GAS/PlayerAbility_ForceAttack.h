#pragma once

#include "CoreMinimal.h"
#include "GAS/PlayerBaseGameplayAbility.h"
#include "PlayerAbility_ForceAttack.generated.h"

/**
 * 
 */
UCLASS()
class TPS_GAS_API UPlayerAbility_ForceAttack : public UPlayerBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UPlayerAbility_ForceAttack();

protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
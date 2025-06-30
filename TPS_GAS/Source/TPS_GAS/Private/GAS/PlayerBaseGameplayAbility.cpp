#include "GAS/PlayerBaseGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "GameplayEffect.h"
#include "TPS_GAS/TPS_GASCharacter.h"

void UPlayerBaseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UAbilitySystemComponent* AbilitySysComp = ActorInfo->AbilitySystemComponent.Get();
	if (!AbilitySysComp)
		return;

	FGameplayEffectContextHandle EffectContext = AbilitySysComp->MakeEffectContext();

	for (auto Effect : OnGoingEffectsToApplyOnStart)
	{
		if (!Effect.Get())
		{
			continue;
		}


		FGameplayEffectSpecHandle SpecHandle = AbilitySysComp->MakeOutgoingSpec(Effect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySysComp->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			if (!ActiveGEHandle.WasSuccessfullyApplied())
			{
				//UE_LOG(LogTemp, Warning, TEXT("Abilty %s failed to apply startup effects % s"), *GetName(), *GetNameSafe(Effect));
				ABILITY_LOG(Log, TEXT("Abilty %s failed to apply startup effects % s"), *GetName(), *GetNameSafe(Effect));
			}
		}
	}

	if (IsInstantiated())
	{
		for (auto Effect : OnGoingEffectsToRemoveOnEnd)
		{
			if (!Effect.Get())
			{
				continue;
			}

			FGameplayEffectSpecHandle SpecHandle = AbilitySysComp->MakeOutgoingSpec(Effect, 1, EffectContext);

			if (SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySysComp->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				if (!ActiveGEHandle.WasSuccessfullyApplied())
				{
					//UE_LOG(LogTemp, Warning, TEXT("Abilty %s failed to apply startup effects % s"), *GetName(), GetNameSafe(Effect));
					ABILITY_LOG(Log, TEXT("Abilty %s failed to apply startup effects % s"), *GetName(), *GetNameSafe(Effect));
				}
				else
				{
					RemoveOnEndEffectHandles.Add(ActiveGEHandle);
				}
			}
		}
	}
}

void UPlayerBaseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsInstantiated())
	{
		UAbilitySystemComponent* AbilitySysComp = ActorInfo->AbilitySystemComponent.Get();
		if (!AbilitySysComp)
			return;

		for (auto EffectHandle : RemoveOnEndEffectHandles) 
		{
			if (!EffectHandle.IsValid())
			{
				continue;
			}

			AbilitySysComp->RemoveActiveGameplayEffect(EffectHandle);
		}
		
		RemoveOnEndEffectHandles.Empty();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

ATPS_GASCharacter* UPlayerBaseGameplayAbility::GetPlayerCharacterFromActorInfo()
{
	return Cast<ATPS_GASCharacter>(GetAvatarActorFromActorInfo());
}

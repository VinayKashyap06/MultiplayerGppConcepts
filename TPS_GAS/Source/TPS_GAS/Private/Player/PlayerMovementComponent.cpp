// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerMovementComponent.h"
#include "Player/PlayerMovementComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GAS/PlayerAbilitySystemComponent.h"

//static TAutoConsoleVariable<int32> CVarShowTraversal(
//	TEXT("ShowDebugTraversal"),
//	0,
//	TEXT("Draw Debug info about traversal"),
//	TEXT("0 : off/n"),
//	TEXT("1 : on/n"),
//	ECVF_Cheat
//);

bool UPlayerMovementComponent::TryTraversal(UPlayerAbilitySystemComponent* AbilitySystemComp)
{
	for (const TSubclassOf<UGameplayAbility>& ability : TraversalAbilitiesOrdered)
	{
		if (AbilitySystemComp->TryActivateAbilityByClass(ability, true))
		{
			FGameplayAbilitySpec* Spec = AbilitySystemComp->FindAbilitySpecFromClass(ability);

			if (Spec && Spec->IsActive())
			{
				return true;
			}
		}
	}

    return false;
}

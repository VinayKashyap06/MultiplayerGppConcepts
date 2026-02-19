#include "Player/PlayerMovementComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GAS/PlayerAbilitySystemComponent.h"

static TAutoConsoleVariable<int32> CVarShowTraversal(
	TEXT("ShowDebugTraversal"),
	0,
	TEXT("Draws debug info about traversal")
	TEXT(" 0: off/n")
	TEXT(" 1: on/n"),
	ECVF_Cheat
);

bool UPlayerMovementComponent::TryTraversal(UPlayerAbilitySystemComponent* AbilitySystemComp)
{
	for (const TSubclassOf<UGameplayAbility>& ability : TraversalAbilitiesOrdered) //try activate all abilities sequential
	{
		if (AbilitySystemComp->TryActivateAbilityByClass(ability, true))
		{
			FGameplayAbilitySpec* Spec = AbilitySystemComp->FindAbilitySpecFromClass(ability);

			if (Spec && Spec->IsActive()) //performed commit check, and then activate ability, could be either vault or jump or whatever was ordered
			{
				return true;
			}
		}
	}

    return false;
}

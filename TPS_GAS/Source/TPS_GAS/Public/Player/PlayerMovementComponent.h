#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovementComponent.generated.h"

class UPlayerAbilitySystemComponent;
class UGameplayAbility;

UCLASS()
class TPS_GAS_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	bool TryTraversal(UPlayerAbilitySystemComponent* AbilitySystemComp);

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> TraversalAbilitiesOrdered;
};

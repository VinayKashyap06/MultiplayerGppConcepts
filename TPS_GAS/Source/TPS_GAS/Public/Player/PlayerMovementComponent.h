// Fill out your copyright notice in the Description page of Project Settings.

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
	UPROPERTY()
	TArray<TSubclassOf<UGameplayAbility>> TraversalAbilitiesOrdered;


};

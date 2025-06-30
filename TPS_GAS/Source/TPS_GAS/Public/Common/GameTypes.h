#pragma once

#include "CoreMinimal.h"
#include "GameTypes.generated.h"

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TArray < TSubclassOf<class UGameplayEffect>> Effects;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TArray < TSubclassOf<class UGameplayAbility>> Abilities;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	class UCharacterAnimDataAsset* CharacterAnimationData;
};


USTRUCT(BlueprintType)
struct FCharacterAnimationData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	class UBlendSpace* MovementBlendSpace;
	
	UPROPERTY(EditDefaultsOnly)
	class UAnimSequenceBase* IdleAnimationSequence;

	UPROPERTY(EditDefaultsOnly)
	class UBlendSpace* CrouchMovementBlendSpace;
	
	UPROPERTY(EditDefaultsOnly)
	class UAnimSequenceBase* CrouchIdleAnimationSequence;
	
};


UENUM(BlueprintType)
enum class E_FootIdentifier : uint8
{
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};
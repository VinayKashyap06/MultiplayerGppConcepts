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
	Left  UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};


//Inventory
class AInventoryItemActor;

UCLASS(BlueprintType, Blueprintable)
class UItemStaticData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AInventoryItemActor> ItemActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SocketToAttachTo = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTransform SpawnTransform;
};

UENUM(BlueprintType)
enum class E_ItemState : uint8
{
	None     UMETA(DisplayName = "None"),
	Equipped UMETA(DisplayName = "Equipped"),
	Dropped  UMETA(DisplayName = "Dropped")
};

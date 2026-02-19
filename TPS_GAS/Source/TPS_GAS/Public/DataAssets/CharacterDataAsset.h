#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Common/GameTypes.h"
#include "CharacterDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TPS_GAS_API UCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	FCharacterData CharacterData;
};

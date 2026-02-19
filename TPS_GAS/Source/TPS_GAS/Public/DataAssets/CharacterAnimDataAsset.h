#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Common/GameTypes.h"
#include "CharacterAnimDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class TPS_GAS_API UCharacterAnimDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FCharacterAnimationData CharacterAnimationData;
};

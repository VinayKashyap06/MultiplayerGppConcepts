#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TPS_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPS_GAS_API UTPS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

};

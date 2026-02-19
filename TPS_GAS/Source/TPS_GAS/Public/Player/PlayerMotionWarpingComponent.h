#pragma once

#include "CoreMinimal.h"
#include "MotionWarpingComponent.h"
#include "PlayerMotionWarpingComponent.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;


//This class decides what is the best action to take (JUMP/VAULT) during parkouring nearby obstacles
UCLASS()
class TPS_GAS_API UPlayerMotionWarpingComponent : public UMotionWarpingComponent
{
	GENERATED_BODY()

};

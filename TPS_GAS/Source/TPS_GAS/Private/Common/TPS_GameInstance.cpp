#include "Common/TPS_GameInstance.h"
#include "AbilitySystemGlobals.h"

void UTPS_GameInstance::Init()
{
	Super::Init();
	UAbilitySystemGlobals::Get().InitGlobalData();
}

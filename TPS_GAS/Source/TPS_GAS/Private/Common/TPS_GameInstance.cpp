// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/TPS_GameInstance.h"
#include "AbilitySystemGlobals.h"

void UTPS_GameInstance::Init()
{
	Super::Init();
	UAbilitySystemGlobals::Get().InitGlobalData();
}

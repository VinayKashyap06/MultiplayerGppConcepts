// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/TPSGasStatics.h"

const UItemStaticData* UTPSGasStatics::GetItemStaticData(TSubclassOf<UItemStaticData> ItemInfo)
{
	if (IsValid(ItemInfo))
	{
		return GetDefault<UItemStaticData>(ItemInfo);
	}
    return nullptr;
}

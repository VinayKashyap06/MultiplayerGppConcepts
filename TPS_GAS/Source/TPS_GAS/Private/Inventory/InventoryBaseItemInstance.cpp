// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryBaseItemInstance.h"


void UInventoryBaseItemInstance::OnRep_IsEquipped()
{

}

void UInventoryBaseItemInstance::Init(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	ItemStaticDataClass = InItemStaticDataClass;
}

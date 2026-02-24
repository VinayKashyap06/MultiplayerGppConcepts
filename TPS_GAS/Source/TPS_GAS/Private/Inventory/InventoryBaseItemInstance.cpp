// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryBaseItemInstance.h"
#include "Net/UnrealNetwork.h"
#include "Common/TPSGasStatics.h"


void UInventoryBaseItemInstance::OnRep_IsEquipped()
{

}

void UInventoryBaseItemInstance::Init(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	ItemStaticDataClass = InItemStaticDataClass;
}

void UInventoryBaseItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryBaseItemInstance, bIsEquipped);
}

const UItemStaticData* UInventoryBaseItemInstance::GetItemStaticData() const
{
	return UTPSGasStatics::GetItemStaticData(ItemStaticDataClass);
}

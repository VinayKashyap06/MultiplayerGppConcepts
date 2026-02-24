// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryRecordList.h"
#include "Common/GameTypes.h"
#include "Inventory/InventoryBaseItemInstance.h"


void FInventoryList::AddItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	FInventoryListItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = NewObject<UInventoryBaseItemInstance>();
	Item.ItemInstance->Init(InItemStaticDataClass);
	
	MarkItemDirty(Item); //marking item dirty is important for fast array serializer
}

void FInventoryList::RemoveItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	for (auto itemItr = Items.CreateIterator(); itemItr; itemItr++)
	{
		FInventoryListItem& Item = *itemItr;

		if (Item.ItemInstance && Item.ItemInstance->IsA(InItemStaticDataClass))
		{
			itemItr.RemoveCurrent();
			MarkArrayDirty(); //marking array dirty is important for fast array serializer, to let it work it's magic
			break;
		}
		
	}
}

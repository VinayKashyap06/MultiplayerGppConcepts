// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "InventoryRecordList.generated.h"

class UInventoryBaseItemInstance;
class UItemStaticData;

USTRUCT(BlueprintType)
struct FInventoryListItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UInventoryBaseItemInstance* ItemInstance = nullptr;

};

USTRUCT(BlueprintType)
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TArray<FInventoryListItem> Items; //Make sure to only have 1 array within FastArraySerializer

public:
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryListItem, FInventoryList>(Items, DeltaParams, *this);
	}

	void AddItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);
	void RemoveItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);

	FORCEINLINE TArray<FInventoryListItem>& GetItemsRef() { return Items; }
};


//This is important to let our custom struct serialized properly

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};
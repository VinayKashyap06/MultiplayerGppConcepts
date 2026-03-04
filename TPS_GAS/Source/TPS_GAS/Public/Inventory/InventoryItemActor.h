// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryItemActor.generated.h"


class UInventoryBaseItemInstance;

UCLASS()
class TPS_GAS_API AInventoryItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AInventoryItemActor();

protected:
	virtual void BeginPlay() override;

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	UInventoryBaseItemInstance* ItemInstance = nullptr;


public:
	virtual void OnEquipped();
	virtual void OnUnequipped();
	virtual void OnDropped();

	void Init(UInventoryBaseItemInstance* InInstance);
};

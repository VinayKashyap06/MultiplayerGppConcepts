// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Common/GameTypes.h"
#include "InventoryBaseItemInstance.generated.h"


UCLASS()
class TPS_GAS_API UInventoryBaseItemInstance : public UObject
{
	GENERATED_BODY()

public:

	virtual void Init(TSubclassOf<UItemStaticData> InItemStaticDataClass);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE virtual bool IsSupportedForNetworking() const override { return true; }


	UFUNCTION(BlueprintCallable)
	const UItemStaticData* GetItemStaticData() const;

	UPROPERTY(Replicated)
	TSubclassOf<UItemStaticData> ItemStaticDataClass;

	UPROPERTY(ReplicatedUsing = OnRep_IsEquipped)
	bool bIsEquipped = false;

	UFUNCTION()
	void OnRep_IsEquipped();

	virtual void OnEquipped() {};
	virtual void OnUnequipped() {};

};

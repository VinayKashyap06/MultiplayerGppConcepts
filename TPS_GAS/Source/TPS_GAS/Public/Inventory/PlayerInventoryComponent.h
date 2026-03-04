#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryRecordList.h"
#include "PlayerInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_GAS_API UPlayerInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerInventoryComponent();

	virtual void InitializeComponent() override;

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//List update
	UFUNCTION(BlueprintCallable)
	void AddItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);
	UFUNCTION(BlueprintCallable)
	void RemoveItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);
	
	//Item instance within llist update
	UFUNCTION(BlueprintCallable)
	void EquipItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);
	UFUNCTION(BlueprintCallable)
	void UnequipItem();
	
	UFUNCTION(BlueprintCallable)
	void DropItem();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventoryBaseItemInstance* GetEquippedItem() const;

	void EquipForceAttackItem();
protected:

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UItemStaticData>> DefaultItems; //default items to provide

	UPROPERTY(Replicated)
	FInventoryList InventoryList; //replicated list which is marked dirty once modified

	UPROPERTY(Replicated)
	UInventoryBaseItemInstance* CurrentEquippedItem = nullptr;


};

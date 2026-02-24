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

	UPROPERTY(Replicated)
	FInventoryList InventoryList;


protected:

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UItemStaticData>> DefaultItems;

};

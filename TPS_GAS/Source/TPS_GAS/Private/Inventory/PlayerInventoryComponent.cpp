#include "Inventory/PlayerInventoryComponent.h"
#include "Inventory/InventoryBaseItemInstance.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"


static TAutoConsoleVariable<int32> CVarShowInventory(
	TEXT("ShowDebugInventory"),
	0,
	TEXT("Draws debug info about inventory")
	TEXT(" 0: off/n")
	TEXT(" 1: on/n"),
	ECVF_Cheat
);

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	//TODO : Vinay maybe set Dormancy too? too much for this proj?
	//SetIsReplicated(true);
}

void UPlayerInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner()->HasAuthority())
	{
		for (auto Item : DefaultItems)
		{
			InventoryList.AddItem(Item);
		}
	}

}

bool UPlayerInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool HasWrittenSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FInventoryListItem& Item : InventoryList.GetItemsRef())
	{
		UInventoryBaseItemInstance* ItemInstance = Item.ItemInstance;

		if (IsValid(ItemInstance))
		{
			HasWrittenSomething = Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}

	return HasWrittenSomething;
}

void UPlayerInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const bool bShowDebug = CVarShowInventory.GetValueOnGameThread() != 0;
	if (bShowDebug)
	{
		for (FInventoryListItem& Item : InventoryList.GetItemsRef())
		{
			UInventoryBaseItemInstance* ItemInstance = Item.ItemInstance;
			const UItemStaticData* ItemStaticData = ItemInstance->GetItemStaticData();
			
			if (IsValid(ItemInstance) && IsValid(ItemStaticData))
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1, 
						0,
						FColor::Green,
						FString::Printf(TEXT("Item : %s"), *ItemStaticData->Name.ToString())
					);
				}

			}
		}

	}
}


void UPlayerInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerInventoryComponent, InventoryList);
}
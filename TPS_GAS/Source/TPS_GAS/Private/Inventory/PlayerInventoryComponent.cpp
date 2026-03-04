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
	//SetIsReplicated(true); <- set via Character class
}

void UPlayerInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner()->HasAuthority())
	{
		for (auto Item : DefaultItems)
		{
			InventoryList.AddItem(Item); //add default items
		}	
	}

}

bool UPlayerInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool HasWrittenSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	//Replicate existing item instances within the replicated list
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
				if (GEngine) //printing item name for now
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

	DOREPLIFETIME(UPlayerInventoryComponent, InventoryList); //inventory list to be replicated
	DOREPLIFETIME(UPlayerInventoryComponent, CurrentEquippedItem); //current item
}

void UPlayerInventoryComponent::AddItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	InventoryList.AddItem(InItemStaticDataClass);
}

void UPlayerInventoryComponent::RemoveItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	InventoryList.RemoveItem(InItemStaticDataClass);
}

void UPlayerInventoryComponent::EquipItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		for (auto Item : InventoryList.GetItemsRef())
		{
			if (Item.ItemInstance->ItemStaticDataClass == InItemStaticDataClass)
			{
				CurrentEquippedItem = Item.ItemInstance;
				CurrentEquippedItem->OnEquipped(GetOwner());
				break;
			}
		}
	}
}

void UPlayerInventoryComponent::UnequipItem()
{
	if (GetOwner()->HasAuthority())
	{
		if (IsValid(CurrentEquippedItem))
		{
			CurrentEquippedItem->OnUnequipped();
		}
	}
}

void UPlayerInventoryComponent::DropItem()
{
	if (GetOwner()->HasAuthority())
	{
		if (IsValid(CurrentEquippedItem))
		{
			CurrentEquippedItem->OnDropItem();
		}
	}
}

UInventoryBaseItemInstance* UPlayerInventoryComponent::GetEquippedItem() const
{
	return CurrentEquippedItem;
}

//This is a test function to check equip/unequip for now
void UPlayerInventoryComponent::EquipForceAttackItem()
{
	if (!CurrentEquippedItem)
	{
		if (InventoryList.GetItemsRef().Num() > 0)
		{
			EquipItem(InventoryList.GetItemsRef()[0].ItemInstance->ItemStaticDataClass);
		}
	}
	else
	{
		CurrentEquippedItem->OnDropItem();
		CurrentEquippedItem = nullptr;
	}
}

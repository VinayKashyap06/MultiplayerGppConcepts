#include "Inventory/InventoryItemActor.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Inventory/InventoryBaseItemInstance.h"

AInventoryItemActor::AInventoryItemActor()
{

}

// Called when the game starts or when spawned
void AInventoryItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AInventoryItemActor::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool HasWrittenSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	
	HasWrittenSomething = Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);

	return HasWrittenSomething;
}

void AInventoryItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInventoryItemActor, ItemInstance);
}

void AInventoryItemActor::OnEquipped() {

}
void AInventoryItemActor::OnUnequipped() {

}
void AInventoryItemActor::OnDropped() {

}

void AInventoryItemActor::Init(UInventoryBaseItemInstance* InInstance)
{

}

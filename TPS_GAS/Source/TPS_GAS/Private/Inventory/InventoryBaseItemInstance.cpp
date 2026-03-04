
#include "Inventory/InventoryBaseItemInstance.h"
#include "Net/UnrealNetwork.h"
#include "Common/TPSGasStatics.h"
#include "Inventory/InventoryItemActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"


void UInventoryBaseItemInstance::OnRep_IsEquipped()
{

}


void UInventoryBaseItemInstance::OnEquipped(AActor* InOwner)
{
	if (!InOwner)
	{
		return;
	}

	if (UWorld * world = InOwner->GetWorld())
	{
		const UItemStaticData* staticData = GetItemStaticData();
		ItemActorInstance = world->SpawnActorDeferred<AInventoryItemActor>(staticData->ItemActorClass, staticData->SpawnTransform, InOwner);
		ItemActorInstance->Init(this);
		ItemActorInstance->FinishSpawning(staticData->SpawnTransform);

		ACharacter* Character = Cast<ACharacter>(InOwner);
		if (USkeletalMeshComponent* SkeletalMesh = Character ? Character->GetMesh() : nullptr )
		{
			//Transform.SetLocation(SkeletalMesh->GetSocketLocation(staticData->SocketToAttachTo));
			ItemActorInstance->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, staticData->SocketToAttachTo);
		}
	}
}

void UInventoryBaseItemInstance::OnUnequipped()
{
	if (ItemActorInstance)
	{
		//TODO VINAY
		// revisit this, check to see if we can use a pool maybe too much for this proj? 
		//let it be destroy and create for now
		ItemActorInstance->Destroy();
		ItemActorInstance = nullptr;
	}
}

void UInventoryBaseItemInstance::Init(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	ItemStaticDataClass = InItemStaticDataClass;
}

void UInventoryBaseItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryBaseItemInstance, ItemStaticDataClass);
	DOREPLIFETIME(UInventoryBaseItemInstance, bIsEquipped);
	DOREPLIFETIME(UInventoryBaseItemInstance, ItemActorInstance);
}

const UItemStaticData* UInventoryBaseItemInstance::GetItemStaticData() const
{
	return UTPSGasStatics::GetItemStaticData(ItemStaticDataClass);
}

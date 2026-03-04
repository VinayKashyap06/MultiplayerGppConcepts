#include "Inventory/InventoryItemActor.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Inventory/InventoryBaseItemInstance.h"

AInventoryItemActor::AInventoryItemActor()
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(StaticMeshComp);
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	SphereComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInventoryItemActor::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComp->SetSimulatePhysics(false);
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
	DOREPLIFETIME(AInventoryItemActor, ItemState);
}

void AInventoryItemActor::OnEquipped() 
{
	ItemState = E_ItemState::Equipped;
}

void AInventoryItemActor::OnUnequipped() 
{
	ItemState = E_ItemState::None;

}

void AInventoryItemActor::OnDropped() 
{
	ItemState = E_ItemState::Dropped;

	GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	if (AActor* ActorOwner = GetOwner())
	{
		const FVector Location = GetActorLocation();
		const FVector OwnerForward = ActorOwner->GetActorForwardVector();
		
		const float DropDistance = 100.0f;
		const float DropTraceDistance = 1000.0f;
		
		const FVector TraceStart = Location * OwnerForward * DropDistance;
		const FVector TraceEnd = TraceStart - FVector::UpVector * DropTraceDistance;

		TArray<AActor*> ActorsToIgnore = { GetOwner() };

		static auto const CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowDebugInventory"));
		const bool bShowInventory = CVar->GetInt() > 0;
		EDrawDebugTrace::Type DrawType = bShowInventory ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None; //debug
		FHitResult OutHitResult;

		if (UKismetSystemLibrary::LineTraceSingleByProfile(this, TraceStart, TraceEnd, TEXT("WorldStatic"), true, ActorsToIgnore, DrawType, OutHitResult, true))
		{
			if (OutHitResult.bBlockingHit)
			{
				SetActorLocation(OutHitResult.Location);
				StaticMeshComp->SetSimulatePhysics(true);
				return;
			}
		}

		SetActorLocation(ActorOwner->GetActorLocation());
	}
}

void AInventoryItemActor::Init(UInventoryBaseItemInstance* InInstance)
{
	ItemInstance = InInstance;
}

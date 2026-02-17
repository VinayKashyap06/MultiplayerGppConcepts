#include "WorldItems/InteractableItem.h"

// Sets default values
AInteractableItem::AInteractableItem()
{
	PrimaryActorTick.bCanEverTick = false;
	SetNetDormancy(ENetDormancy::DORM_Initial);
}

void AInteractableItem::BeginPlay()
{
	Super::BeginPlay();
	bNetLoadOnClient = true;
	bReplicates = true;
}

void AInteractableItem::OnForceApplied(const AActor* PlayerInstigator, const float& ForceApplied)
{
	SetNetDormancy(ENetDormancy::DORM_Awake);
	SetReplicateMovement(true);

	//here we can determine what sort of action can be taken with this force and instigator

	const FVector& SourceLocation = PlayerInstigator->GetActorLocation();
	const FVector& ForceDirection = (GetActorLocation() - SourceLocation).GetSafeNormal();
	UPrimitiveComponent* TargetComp = Cast<UPrimitiveComponent>(RootComponent);
	if (TargetComp && TargetComp->IsSimulatingPhysics())
	{
		TargetComp->AddImpulse(ForceDirection * ForceApplied);
	}
}


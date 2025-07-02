
#include "WorldItems/DestructibleItem.h"

ADestructibleItem::ADestructibleItem()
{
	PrimaryActorTick.bCanEverTick = false;

	//here we add our physics field
	GeometryCollectionComp = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("DestructibleGeometryCollection"));
	SetRootComponent(GeometryCollectionComp);
	GeometryCollectionComp->SetSimulatePhysics(true);

	GeometryCollectionComp->PutAllRigidBodiesToSleep();

}

//Instead of applying impulse on root comp, apply it and fracture the sphere
void ADestructibleItem::OnForceApplied(const AActor* PlayerInstigator, const float& ForceApplied)
{
	UE_LOG(LogTemp, Warning, TEXT("damage applied %f"), ForceApplied);
	FVector LocationToUse = GeometryCollectionComp->GetCenterOfMass();
	//FVector LocationToUse = GetActorLocation();

	DrawDebugSphere(GetWorld(), LocationToUse, 50.0f, 50, FColor::Blue, true, 5.0f);

	GeometryCollectionComp->WakeAllRigidBodies();

	// Add impulse at location
	GeometryCollectionComp->AddImpulseAtLocation(FVector::OneVector * ForceApplied, GetActorLocation());
}

void ADestructibleItem::BeginPlay()
{
	Super::BeginPlay();

	GeometryCollectionComp->OnChaosBreakEvent.AddDynamic(this, &ADestructibleItem::OnBreakEvent);
}

//This comes from individual pieces as well
//Just store info for now
void ADestructibleItem::OnBreakEvent(const FChaosBreakEvent& BreakEvent)
{
	// Store data for each broken piece
	StorePieceData(BreakEvent);
	UE_LOG(LogTemp, Warning, TEXT("Piece %d broke at %s"),
		BreakEvent.Index, *BreakEvent.Location.ToString());
}

void ADestructibleItem::StorePieceData(const FChaosBreakEvent& BreakEvent)
{
	// Store transform and velocity data
	FTransform PieceTransform;

	PieceTransform.SetLocation(BreakEvent.Location);
	BrokenPieceTransforms.Add(PieceTransform);

	BrokenPieceVelocities.Add(BreakEvent.Velocity);
	BrokenPieceIndices.Add(BreakEvent.Index);
}
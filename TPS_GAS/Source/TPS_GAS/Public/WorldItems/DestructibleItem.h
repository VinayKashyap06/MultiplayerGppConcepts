// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldItems/InteractableItem.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

#include "DestructibleItem.generated.h"

/**
 * 
 */
UCLASS()
class TPS_GAS_API ADestructibleItem : public AInteractableItem
{
	GENERATED_BODY()

public:
	ADestructibleItem();

	virtual void OnForceApplied(const AActor* PlayerInstigator, const float& ForceApplied) override;

	virtual void BeginPlay() override;
 protected:

	UPROPERTY(VisibleAnywhere, Category = "ChaosPhysics")
	class UGeometryCollectionComponent* GeometryCollectionComp;

	// Event callbacks
	UFUNCTION()
	void OnBreakEvent(const FChaosBreakEvent& BreakEvent);

private:
	
	// Track broken pieces for manipulation
	UPROPERTY()
	TArray<FTransform> BrokenPieceTransforms;

	UPROPERTY()
	TArray<FVector> BrokenPieceVelocities;

	UPROPERTY()
	TSet<int32> BrokenPieceIndices;

	// Helper functions
	void StorePieceData(const FChaosBreakEvent& BreakEvent);
};

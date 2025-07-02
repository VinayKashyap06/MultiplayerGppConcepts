// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Common/Interfaces/InteractableInterface.h"
#include "InteractableItem.generated.h"

UCLASS()
class TPS_GAS_API AInteractableItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void OnForceApplied(const AActor* PlayerInstigator, const float& ForceApplied) override;
};

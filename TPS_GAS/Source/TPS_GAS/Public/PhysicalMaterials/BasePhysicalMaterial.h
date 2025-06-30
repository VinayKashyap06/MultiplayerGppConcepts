// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "BasePhysicalMaterial.generated.h"

/**
 * 
 */
UCLASS()
class TPS_GAS_API UBasePhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sound")
	class USoundBase* FootstepSound;

public:
	FORCEINLINE  class USoundBase* GetFootStepSound() { return FootstepSound; }
};

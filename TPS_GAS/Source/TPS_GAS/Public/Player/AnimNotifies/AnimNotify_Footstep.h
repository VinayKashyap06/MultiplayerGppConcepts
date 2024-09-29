// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Common/GameTypes.h"

#include "AnimNotify_Footstep.generated.h"

/**
 * 
 */
UCLASS()
class TPS_GAS_API UAnimNotify_Footstep : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere)
	E_FootIdentifier Foot;
};

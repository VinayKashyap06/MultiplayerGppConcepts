// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AnimNotifies/AnimNotify_Footstep.h"

#include "TPS_GAS/TPS_GASCharacter.h"
#include "Player/PlayerAudioComponent.h"

void UAnimNotify_Footstep::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation)
{
	Super::Notify(Mesh, Animation);
	
	check(Mesh);

	ATPS_GASCharacter* Character = Mesh ? Cast<ATPS_GASCharacter>(Mesh->GetOwner()) : nullptr;

	if (Character)
	{
		if (Character->GetPlayerAudioComp()) 
		{
			Character->GetPlayerAudioComp()->HandleFootStepSound(Foot);
		}
	}
}

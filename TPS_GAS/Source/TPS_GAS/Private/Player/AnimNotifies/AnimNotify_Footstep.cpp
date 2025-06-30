// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AnimNotifies/AnimNotify_Footstep.h"

#include "TPS_GAS/TPS_GASCharacter.h"
#include "Player/PlayerAudioComponent.h"

//Playing audio via notify
void UAnimNotify_Footstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	check(MeshComp);

	ATPS_GASCharacter* Character = MeshComp ? Cast<ATPS_GASCharacter>(MeshComp->GetOwner()) : nullptr;

	if (Character)
	{
		if (Character->GetPlayerAudioComp())
		{
			Character->GetPlayerAudioComp()->HandleFootStepSound(Foot);
		}
	}
}

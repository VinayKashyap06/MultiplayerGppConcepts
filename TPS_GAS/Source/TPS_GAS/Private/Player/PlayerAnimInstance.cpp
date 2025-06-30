// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerAnimInstance.h"
#include "TPS_GAS/TPS_GASCharacter.h"
#include "DataAssets/CharacterAnimDataAsset.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/BlendSpace.h"
#include "Common/GameTypes.h"

UBlendSpace* UPlayerAnimInstance::GetLocomotionBlendSpace() const
{
	if (ATPS_GASCharacter* Character = Cast<ATPS_GASCharacter>(GetOwningActor()))
	{
		FCharacterData Data = Character->GetCharacterData();
		if (Data.CharacterAnimationData)
		{
			return Data.CharacterAnimationData->CharacterAnimationData.MovementBlendSpace;
		}
	}
	return DefaultCharacterAnimData ? DefaultCharacterAnimData->CharacterAnimationData.MovementBlendSpace : nullptr;
}

UAnimSequenceBase* UPlayerAnimInstance::GetIdleAnimationSequence() const
{
	if (ATPS_GASCharacter* Character = Cast<ATPS_GASCharacter>(GetOwningActor()))
	{
		FCharacterData Data = Character->GetCharacterData();
		if (Data.CharacterAnimationData)
		{
			return Data.CharacterAnimationData->CharacterAnimationData.IdleAnimationSequence;
		}
	}

	return DefaultCharacterAnimData ? DefaultCharacterAnimData->CharacterAnimationData.IdleAnimationSequence : nullptr;
}

UBlendSpace* UPlayerAnimInstance::GetCrouchLocomotionBlendSpace() const
{
	if (ATPS_GASCharacter* Character = Cast<ATPS_GASCharacter>(GetOwningActor()))
	{
		FCharacterData Data = Character->GetCharacterData();
		if (Data.CharacterAnimationData)
		{
			return Data.CharacterAnimationData->CharacterAnimationData.CrouchMovementBlendSpace;
		}
	}
	return DefaultCharacterAnimData ? DefaultCharacterAnimData->CharacterAnimationData.CrouchMovementBlendSpace : nullptr;
}

UAnimSequenceBase* UPlayerAnimInstance::GetCrouchIdleAnimationSequence() const
{
	if (ATPS_GASCharacter* Character = Cast<ATPS_GASCharacter>(GetOwningActor()))
	{
		FCharacterData Data = Character->GetCharacterData();
		if (Data.CharacterAnimationData)
		{
			return Data.CharacterAnimationData->CharacterAnimationData.CrouchIdleAnimationSequence;
		}
	}

	return DefaultCharacterAnimData ? DefaultCharacterAnimData->CharacterAnimationData.CrouchIdleAnimationSequence : nullptr;
}

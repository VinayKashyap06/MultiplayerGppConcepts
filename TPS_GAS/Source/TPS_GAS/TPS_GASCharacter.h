// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include <AbilitySystemComponent.h>
#include "AbilitySystemInterface.h"
#include "Common/GameTypes.h"


#include "TPS_GASCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UPlayerAbilitySystemComponent;
class UPlayerBaseAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UPlayerMotionWarpingComponent;
class UPlayerMovementComponent;
struct FInputActionValue;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ATPS_GASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	
	ATPS_GASCharacter(const FObjectInitializer& ObjectInitializer);
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	/** Called for jump input */
	void OnJump(const FInputActionValue& Value);

	void OnStopJumping(const FInputActionValue& Value);

	void ToggleCrouch(const FInputActionValue& Value);

	void OnSprintStarted(const FInputActionValue& Value);
	void OnSprintStopped(const FInputActionValue& Value);


	void GiveAbilities();
	void ApplyStartupEffects();

	//overriden methods
	virtual void Landed(const FHitResult& Hit) override; //to remove jump effect
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> DefaultAttributeSet;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	UPlayerAbilitySystemComponent* PlayerAbilitySystemComp;
	UPROPERTY(Transient,VisibleAnywhere)
	UPlayerBaseAttributeSet* PlayerBaseAttributes;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(Replicatedusing = OnRep_CharacterData)
	FCharacterData CharacterData;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Audio")
	class UPlayerAudioComponent* PlayerAudioComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
	UPlayerMovementComponent* PlayerMovementComponent;
	
	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
	UPlayerMotionWarpingComponent* PlayerMotionWarpingComponent;

protected:

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UPlayerMotionWarpingComponent* GetMotionWarpingComponent() const { return PlayerMotionWarpingComponent; }

	//GAS helper
	bool ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle InEffectContext);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FCharacterData GetCharacterData() const { return CharacterData; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCharacterData(const FCharacterData& InCharacterData) { CharacterData = InCharacterData; InitFromCharacterData(CharacterData); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UPlayerAudioComponent* GetPlayerAudioComp() const { return PlayerAudioComponent; }

	void SetCrouchedCamera(bool set);


	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data);

protected:
	UFUNCTION()
	void OnRep_CharacterData();

	virtual void InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication = false);

	UPROPERTY(EditDefaultsOnly)
	class UCharacterDataAsset* CharacterDataAsset;

protected:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag JumpEventTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer InAirTags; //to remove/activate all in air tags

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer CrouchTags; //to remove/activate all crouched tags
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer SprintTags; //to remove/activate all sprint tags

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> CrouchStateEffect;


	//Delegates
	FDelegateHandle MaxMovementSpeedChangedDelegateHandle;

};


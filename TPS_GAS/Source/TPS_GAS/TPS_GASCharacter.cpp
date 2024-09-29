// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPS_GASCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/PlayerAbilitySystemComponent.h"
#include "GAS/PlayerBaseAttributeSet.h"

#include "DataAssets/CharacterDataAsset.h"
#include "Player/PlayerMovementComponent.h"
#include "Player/PlayerAudioComponent.h"
#include "Player/PlayerMotionWarpingComponent.h"

#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATPS_GASCharacter

ATPS_GASCharacter::ATPS_GASCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)



	//GAS
	PlayerAbilitySystemComp = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(TEXT("PlayerAbilityComp"));
	PlayerAbilitySystemComp->SetIsReplicated(true);
	PlayerAbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	//Binding change in attribute to our custom code
	PlayerAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(PlayerBaseAttributes->GetMaxMovementSpeedAttribute()).AddUObject(this, &ThisClass::OnMaxMovementSpeedChanged);

	PlayerBaseAttributes = CreateDefaultSubobject<UPlayerBaseAttributeSet>(TEXT("PlayerBaseAttributes"));
	
	PlayerAudioComponent = CreateDefaultSubobject<UPlayerAudioComponent>(TEXT("PlayerAudioComp"));
	PlayerMovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
	PlayerMotionWarpingComponent = CreateDefaultSubobject<UPlayerMotionWarpingComponent>(TEXT("PlayerMotionWarpingComp"));
}

void ATPS_GASCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ATPS_GASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(CharacterDataAsset))
	{
		SetCharacterData(CharacterDataAsset->CharacterData);
	}
}

void ATPS_GASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPS_GASCharacter, CharacterData);
}

bool ATPS_GASCharacter::ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle InEffectContext)
{
	if (!Effect.Get())
	{
		return false;
	}

	FGameplayEffectSpecHandle SpecHandle = PlayerAbilitySystemComp->MakeOutgoingSpec(Effect, 1, InEffectContext);
	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = PlayerAbilitySystemComp->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		return ActiveGEHandle.WasSuccessfullyApplied();
	}

	return false;
}


UAbilitySystemComponent* ATPS_GASCharacter::GetAbilitySystemComponent() const
{
	return PlayerAbilitySystemComp;
}


//////////////////////////////////////////////////////////////////////////
// Input

void ATPS_GASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATPS_GASCharacter::OnJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATPS_GASCharacter::OnStopJumping);

		//Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ATPS_GASCharacter::ToggleCrouch);
		
		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ATPS_GASCharacter::OnSprintStarted);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ATPS_GASCharacter::OnSprintStopped);


		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPS_GASCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPS_GASCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATPS_GASCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATPS_GASCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

//GAS + input
void ATPS_GASCharacter::OnJump(const FInputActionValue& Value)
{
	//SEND EVENT, observe this event in ability
	FGameplayEventData NewPayload;
	NewPayload.EventTag = JumpEventTag;
	NewPayload.Instigator = this;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, JumpEventTag, NewPayload);


	//Calling movement component's traversal to allow motion warping
	if(PlayerAbilitySystemComp)
		PlayerMovementComponent->TryTraversal(PlayerAbilitySystemComp);
}

void ATPS_GASCharacter::OnStopJumping(const FInputActionValue& Value)
{
	StopJumping();
}

void ATPS_GASCharacter::Landed(const FHitResult& Hit) //overriden
{
	Super::Landed(Hit);

	if (PlayerAbilitySystemComp)
	{
		PlayerAbilitySystemComp->RemoveActiveEffectsWithTags(InAirTags);
	}
}

void ATPS_GASCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void ATPS_GASCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void ATPS_GASCharacter::ToggleCrouch(const FInputActionValue& Value)
{
	//apply crouch ability if uncrouched
	if (bIsCrouched)
	{
		//uncrouch
		PlayerAbilitySystemComp->CancelAbilities(&CrouchTags);
		return;
	}

	//Send Direct call
	PlayerAbilitySystemComp->TryActivateAbilitiesByTag(CrouchTags, true);
}

void ATPS_GASCharacter::OnSprintStarted(const FInputActionValue& Value)
{
	if (PlayerAbilitySystemComp)
	{
		PlayerAbilitySystemComp->TryActivateAbilitiesByTag(SprintTags, true);
	}
}

void ATPS_GASCharacter::OnSprintStopped(const FInputActionValue& Value)
{
	if (PlayerAbilitySystemComp)
	{
		PlayerAbilitySystemComp->CancelAbilities(&SprintTags);
	}
}


////Gameplay Abilities

void ATPS_GASCharacter::GiveAbilities()
{
	if (HasAuthority() && PlayerAbilitySystemComp)
	{
		for (auto& DefaultAbility : CharacterData.Abilities)
		{
			PlayerAbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DefaultAbility));
		}
		UE_LOG(LogTemp, Warning, TEXT("abilities given"));
	}
}

void ATPS_GASCharacter::ApplyStartupEffects()
{
	if (HasAuthority() && PlayerAbilitySystemComp)
	{
		//create context handle
		FGameplayEffectContextHandle EffectContext = PlayerAbilitySystemComp->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		for (auto& StartupEffect : CharacterData.Effects)
		{
			ApplyGameplayEffectToSelf(StartupEffect, EffectContext); //our helper function
		}
		UE_LOG(LogTemp, Warning, TEXT("startupeffects attributes"));
	}
}

//Player Replication
//runs on server
void ATPS_GASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PlayerAbilitySystemComp->InitAbilityActorInfo(this, this);
	GiveAbilities();
	ApplyStartupEffects();

}

//replicated to client so client can init ability
void ATPS_GASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	PlayerAbilitySystemComp->InitAbilityActorInfo(this, this);
}

void ATPS_GASCharacter::SetCrouchedCamera(bool set)
{
	CameraBoom->SocketOffset.Y = set? -20.0f : 0.0f;
}

void ATPS_GASCharacter::OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

void ATPS_GASCharacter::OnRep_CharacterData()
{
	InitFromCharacterData(CharacterData, true);
}

void ATPS_GASCharacter::InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication)
{

}

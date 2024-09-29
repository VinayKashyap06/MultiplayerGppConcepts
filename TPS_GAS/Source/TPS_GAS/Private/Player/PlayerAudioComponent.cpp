#include "Player/PlayerAudioComponent.h"

#include "TPS_GAS/TPS_GASCharacter.h"
#include "PhysicalMaterials/BasePhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<int32> CVarShowFootsteps(
	TEXT("ShowDebugFootsteps"),
	0,
	TEXT("Draws debug info about footsteps")
	TEXT("  0: off/n")
	TEXT("  1: on/n"),
	ECVF_Cheat);


UPlayerAudioComponent::UPlayerAudioComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UPlayerAudioComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerAudioComponent::HandleFootStepSound(E_FootIdentifier FootID)
{
	if (ATPS_GASCharacter* Character = Cast<ATPS_GASCharacter>( GetOwner()))
	{
		const int32 DebugShowFootsteps = CVarShowFootsteps.GetValueOnAnyThread();
		if (USkeletalMeshComponent* Mesh =  Character->GetMesh())
		{
			FHitResult HitResult;
			const FVector SocketLocation = Mesh->GetSocketLocation(FootID == E_FootIdentifier::Left ? LeftFootSocketTag : RightFootSocketTag);
			const FVector StartLocation = SocketLocation + FVector::UpVector * 20.0f;
			const FVector EndLocation = StartLocation + FVector::UpVector * -50.0f;

			FCollisionQueryParams QueryParams;
			QueryParams.bReturnPhysicalMaterial = true;

			QueryParams.AddIgnoredActor(Character);

			if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation,  EndLocation, ECollisionChannel::ECC_WorldStatic, QueryParams))
			{
				if (HitResult.bBlockingHit)
				{
					if (HitResult.PhysMaterial.Get())
					{
						UBasePhysicalMaterial* PhysicalMat = Cast<UBasePhysicalMaterial>(HitResult.PhysMaterial.Get());
						if (PhysicalMat)
						{
							UGameplayStatics::PlaySoundAtLocation(this, PhysicalMat->GetFootStepSound(), StartLocation);

							if ( DebugShowFootsteps > 0)
							{
								DrawDebugString(GetWorld(), StartLocation, PhysicalMat->GetName(), nullptr, FColor::White, 4.0f);
							}
						}
					}
				}

				if ( DebugShowFootsteps > 0)
				{
					DrawDebugSphere(GetWorld(), StartLocation, 16, 16, FColor::Red, false, 4.0f);
				}
			}
			else
			{
				if (DebugShowFootsteps > 0)
				{
					DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 4.0f);
				}

			}
		}
	}
}

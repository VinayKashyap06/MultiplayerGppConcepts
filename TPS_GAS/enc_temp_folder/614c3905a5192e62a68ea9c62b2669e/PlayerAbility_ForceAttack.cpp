#include "GAS/PlayerAbility_ForceAttack.h"
#include "Common/Interfaces/InteractableInterface.h"
#include <Kismet/GameplayStatics.h>
#include <Common/Utils/Utils.h>
#include <TPS_GAS/TPS_GASCharacter.h>


UPlayerAbility_ForceAttack::UPlayerAbility_ForceAttack()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UPlayerAbility_ForceAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return true;
}

void UPlayerAbility_ForceAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//activate force
	//Do a cone trace
	const FVector& Location = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	const FVector& Forward = GetPlayerCharacterFromActorInfo()->GetActorForwardVector();
	TArray<AActor*> OutActorsInCone;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic)); //world dynamic for now

	OutActorsInCone = GetActorsInCone(GetWorld(), Location, Forward, 1000.0f, 45.0f, ObjectTypes, OutActorsInCone, true); //todo shift the constants to uproperties


	for (AActor* Target : OutActorsInCone)
	{
		if (Target->GetClass()->ImplementsInterface(IInteractableInterface::UClassType::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("INteractable actor in cone: %s"), *Target->GetName());
			IInteractableInterface* interactable = Cast<IInteractableInterface>(Target);
			if (interactable)
			{
				interactable->OnForceApplied(GetPlayerCharacterFromActorInfo(), 100.0f);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Regular actor in cone: %s"), *Target->GetName());
		}
	}
	//apply animation
	
	//apply physics
	ApplyCooldown(Handle, ActorInfo, ActivationInfo);
	//Attack done
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UPlayerAbility_ForceAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	//stop ability
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
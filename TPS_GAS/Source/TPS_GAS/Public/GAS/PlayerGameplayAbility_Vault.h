#pragma once

#include "CoreMinimal.h"
#include "GAS/PlayerBaseGameplayAbility.h"
#include "PlayerGameplayAbility_Vault.generated.h"

/**
 * 
 */
UCLASS()
class TPS_GAS_API UPlayerGameplayAbility_Vault : public UPlayerBaseGameplayAbility
{
	GENERATED_BODY()
	UPlayerGameplayAbility_Vault();
	
public:

protected:
	//Check if everything is valid 
	//this happens after CanActivateAbility, sort of two factor auth
	//we have to explicitly call this from Activate ability
	virtual bool CommitCheck(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags) override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "TraceSettings")
	float HorizontalTraceRadius = 30.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "TraceSettings")
	float HorizontalTraceLength = 500.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "TraceSettings")
	float HorizontalTraceCount = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "TraceSettings")
	float HorizontalTraceStep = 30.0f;
	
	
	UPROPERTY(EditDefaultsOnly, Category = "TraceSettings")
	float VerticalTraceRadius = 30.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "TraceSettings")
	float VerticalTraceLength = 500.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "TraceSettings")
	float VerticalTraceCount = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "TraceSettings")
	float VerticalTraceStep = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category = "TraceSettings")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes; //collisions to trace

	UPROPERTY(EditDefaultsOnly, Category = "TraceSettings")
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannelsToIgnore; //collisions to ignore
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* VaultMontage = nullptr; //jumpover montage

	UPROPERTY()
	class UAbilityTask_PlayMontageAndWait* MontagePlayAwaitTask = nullptr;

	//cached target points during commit check
	FVector JumpOverLocation= FVector::Zero();
	FVector JumpToLocation = FVector::Zero();

};

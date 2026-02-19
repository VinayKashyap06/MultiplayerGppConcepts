#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "PlayerAbilityTask_WallRun.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWallRunWallSideFoundDelegate, bool, bLeftSide);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWallRunWallFinishedDelegate);

//Reference - 
// https://dev.epicgames.com/documentation/en-us/unreal-engine/gameplay-ability-tasks-in-unreal-engine
// https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Plugins/GameplayAbilities/UAbilityTask
//Use this ability task since this wall running is an ongoing behaviour
UCLASS()
class TPS_GAS_API UPlayerAbilityTask_WallRun : public UAbilityTask
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnWallRunWallFinishedDelegate OnWallRunFinished;
	
	UPROPERTY(BlueprintAssignable)
	FOnWallRunWallSideFoundDelegate OnWallFound;

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta= (HiddenPin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UPlayerAbilityTask_WallRun* CreateWallRunTask(UGameplayAbility* OwningAbility, ACharacter* CharacterOwner, class UCharacterMovementComponent* CharacterMovement, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes);

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	virtual void TickTask(float DeltaTime) override;

protected:
	UCharacterMovementComponent* CharacterMovement = nullptr;
	ACharacter* CharacterOwner = nullptr;
	TArray<TEnumAsByte<EObjectTypeQuery>> WallRun_TraceObjectTypes;

	bool FindRunnableWall(FHitResult& OnWallHit);

	bool IsWallOnTheLeft(const FHitResult& InWallHit) const;

};

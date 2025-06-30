#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/GameTypes.h"

#include "PlayerAudioComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_GAS_API UPlayerAudioComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerAudioComponent();

	void HandleFootStepSound(E_FootIdentifier FootID);
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	FName LeftFootSocketTag = FName("foot_l");
	
	UPROPERTY(EditDefaultsOnly)
	FName RightFootSocketTag = FName("foot_r");

};

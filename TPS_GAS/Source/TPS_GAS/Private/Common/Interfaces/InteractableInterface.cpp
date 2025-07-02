#include "Common/Interfaces/InteractableInterface.h"

// Add default functionality here for any IInteractableInterface functions that are not pure virtual.

void IInteractableInterface::OnForceApplied(const AActor* PlayerInstigator, const float& ForceApplied)
{
	if (PlayerInstigator)
	{
		UE_LOG(LogTemp, Warning, TEXT("Force Applied by instigator %s"), *PlayerInstigator->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Force Applied but no instigator"));
	}
}

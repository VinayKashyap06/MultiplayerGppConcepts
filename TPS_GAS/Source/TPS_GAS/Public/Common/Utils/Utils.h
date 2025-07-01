#pragma once

#include <Kismet/GameplayStatics.h>
#include <DrawDebugHelpers.h>

// <summary>
///Get all actors in a radius, then check if they fall within a cone angle from the forward direction.
/// </summary>
/// <param name="World"></param>
/// <param name="Origin"></param>
/// <param name="Forward"></param>
/// <param name="Radius"></param>
/// <param name="HalfAngleDegrees"></param>
/// <param name="ObjectTypes"></param>
/// <param name="OutActors"></param>
/// <returns></returns>
TArray<AActor*> GetActorsInCone(
	const UWorld* World,
	FVector Origin,
	FVector Forward,
	float Radius,
	float HalfAngleDegrees,
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes, //for sphere overlap
	TArray<AActor*>& OutActors,
	bool bEnableDebug
)
{
	TArray<AActor*> OverlappingActors;
	TArray<AActor*> ActorsToIgnore;

	// Get all overlapping actors using sphere overlap
	UKismetSystemLibrary::SphereOverlapActors(
		World,
		Origin,
		Radius,
		ObjectTypes,
		AActor::StaticClass(),
		ActorsToIgnore, // Actors to ignore
		OverlappingActors
	);

	float CosThreshold = FMath::Cos(FMath::DegreesToRadians(HalfAngleDegrees)); //cos of half angles

	for (AActor* Actor : OverlappingActors)
	{
		FVector ToTarget = (Actor->GetActorLocation() - Origin).GetSafeNormal(); //direction
		float Dot = FVector::DotProduct(Forward.GetSafeNormal(), ToTarget); //dot product

		if (Dot >= CosThreshold)
		{
			OutActors.Add(Actor);
		}
	}

	if (bEnableDebug)
	{
		DrawDebugCone(
			World, 
			Origin, 
			Forward,
			Radius,
			FMath::DegreesToRadians(HalfAngleDegrees),
			FMath::DegreesToRadians(HalfAngleDegrees),
			12,
			FColor::Green,
			false,
			2.0f);
	}

	return OutActors;
}
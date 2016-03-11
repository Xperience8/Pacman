// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanPickupSpawnerComponent.h"

#include "../Pickups/Boosts/PacmanBoostGrow.h"
#include "../Pickups/Boosts/PacmanBoostShooting.h"
#include "../Pickups/Collectibles/PacmanCollectible.h"

// Sets default values for this component's properties
UPacmanPickupSpawnerComponent::UPacmanPickupSpawnerComponent() : SpawnHeight(30.f), RadiusAroundPlayer(100.f), NumBoosts(10)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPacmanPickupSpawnerComponent::Spawn(const FColor * Pixels, int32 SizeX, int32 SizeY, float StartPixel, float Step, FVector PixelToWorld, FVector WorldStart)
{
	TActorIterator<APlayerStart> PlayerStart(GetWorld());

	// Ensure, we have everything what we need
	if (!CollectibleTemplate || !BoostGrowTemplate || !BoostShootingTemplate || !PlayerStart)
	{
		return;
	}

	// Generate pickup locations from World map
	TArray<FVector> Locations;
	for (float Y = StartPixel; Y < static_cast<float>(SizeY); Y += Step)
	{
		for (float X = StartPixel; X < static_cast<float>(SizeX); X += Step)
		{
			if (Pixels[static_cast<int32>(Y)* SizeX + static_cast<int32>(X)].R == 255)
			{
				FVector Location = FVector(X, Y, SpawnHeight) * PixelToWorld + WorldStart;

				// Skip locations which are too close to player(or inside player)
				if ((Location - PlayerStart->GetActorLocation()).SizeSquared2D() > RadiusAroundPlayer)
				{
					Locations.Add(Location);
				}
			}
		}
	}

	SpawnBoosts(Locations);
	SpawnCollectibles(Locations);
}

void UPacmanPickupSpawnerComponent::SpawnBoosts(TArray<FVector>& Locations)
{
	if (NumBoosts > Locations.Num())
	{
		NumBoosts = Locations.Num();
	}

	FMath::SRandInit(FMath::SRand());

	// How many locations we need to skip to spawn another boost
	const int32 LocationsPerBoost = (Locations.Num() / NumBoosts);

	const int32 NumBoostTypes = 2;
	for (int32 i = 0; i < NumBoosts; ++i)
	{
		int32 PickupId = i * LocationsPerBoost + FMath::RandRange(0, LocationsPerBoost - 1);
		check(PickupId < Locations.Num());

		if (i % NumBoostTypes == 0)
		{
			World->SpawnActor<APacmanBoostGrow>(BoostGrowTemplate, Locations[PickupId], FRotator(0.f, 0.f, 0.f));
		}
		else if (i % NumBoostTypes == 1)
		{
			World->SpawnActor<APacmanBoostShooting>(BoostShootingTemplate, Locations[PickupId], FRotator(0.f, 0.f, 0.f));
		}

		// Bring already taken location to the front of the list 
		Locations.Swap(i, PickupId);
	}
}

void UPacmanPickupSpawnerComponent::SpawnCollectibles(TArray<FVector>& Locations)
{
	NumCollectibles = Locations.Num() - NumBoosts;
	for (int32 i = NumBoosts; i < Locations.Num(); ++i)
	{
		World->SpawnActor<APacmanCollectible>(CollectibleTemplate, Locations[i], FRotator(0.f, 0.f, 0.f));
	}
}


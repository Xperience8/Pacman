// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "PacmanPickupSpawnerComponent.generated.h"


/// PacmanPickupSpawnerComponent is responsible for spawning pickups across the world
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PACMAN_API UPacmanPickupSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPacmanPickupSpawnerComponent();

	/// Spawn pickups based on world map data
	void Spawn(const FColor *Pixels, int32 SizeX, int32 SizeY, float StartPixel, float Step, FVector PixelToWorld, FVector WorldStart);

	/// Returns how many collectibles were spawned
	int32 GetNumCollectibles() const
	{
		return NumCollectibles;
	}

private:
	/// Spread boosts over map
	/// Every location which is used by boost is moved to the front of the array
	/// Therefore, after this call, the first NumBoosts locations are already used and should be skipped
	/// This method should be called before SpawnCollectibles!
	void SpawnBoosts(TArray<FVector> &Locations);

	/// Put collectibles inside remaining locations
	/// NumBoosts locations from the start are skipped, because they have already been used by boosts
	/// This method should be called after SpawnBoosts!
	void SpawnCollectibles(TArray<FVector> &Locations);

	UPROPERTY(EditAnywhere, Category = "Templates")
		TSubclassOf<class APacmanCollectible> CollectibleTemplate;

	UPROPERTY(EditAnywhere, Category = "Templates")
		TSubclassOf<class APacmanBoostGrow> BoostGrowTemplate;

	UPROPERTY(EditAnywhere, Category = "Templates")
		TSubclassOf<class APacmanBoostShooting> BoostShootingTemplate;

	/// Invisible sphere around player in which pickups won't be spawned
	UPROPERTY(EditAnywhere, Category = "Parameters")
		float RadiusAroundPlayer;

	/// In which height, from the ground, are pickups spawned
	UPROPERTY(EditAnywhere, Category = "Parameters")
		float SpawnHeight;

	/// How many boosts are spawned
	UPROPERTY(EditAnywhere, Category = "Parameters")
		int32 NumBoosts;

	/// How many collectibles are spawned
	UPROPERTY(VisibleAnywhere, Category = "Parameters")
		int32 NumCollectibles;
};

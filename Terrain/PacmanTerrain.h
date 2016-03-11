// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "PacmanTerrain.generated.h"

/// PacmanTerrain is responsible for creating terrain and spawning pickups and navigation nodes inside it
UCLASS()
class PACMAN_API APacmanTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	/// Sets default values for this actor's properties
	APacmanTerrain();

	/// Spawn pickups and navigation nodes 
	virtual void PostInitializeComponents() override;

	/// Returns pickup spawner component which can not be null
	class UPacmanPickupSpawnerComponent *GetPickupSpawner() const
	{
		return PickupSpawner;
	}

#if WITH_EDITOR
	/// If we are changing world map, which represents this terrain, then we need to recompute block size
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:
	/// Block is square(in pixel coordinates) in which we can place pickup
	/// There can be only one pickup or navigation node in one block
	/// and only one block can be between sides
	/// ___________________________
	///	 B	B	B	B	B	B	B		
	///	    _______________________
	///  B |
	void FindBlockSize(const FColor *Pixels, int32 MapWidth, int32 MapHeight);
#endif

private:
	/// Holds information about world
	/// Black pixels are blocked, white pixels are used for spawning pickups and navigation nodes
	UPROPERTY(EditAnywhere)
		UTexture2D *WorldMap;

	/// Map is decomposed into blocks(in one block we can put only one pickup) and this is size in pixels of one block
	UPROPERTY(VisibleAnywhere)
		int32 BlockSize;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent *Mesh;

	UPROPERTY(VisibleAnywhere)
	class UPacmanNavigationSpawnerComponent *NavigationSpawner;

	UPROPERTY(VisibleAnywhere)
	class UPacmanPickupSpawnerComponent *PickupSpawner;
};

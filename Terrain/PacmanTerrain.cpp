// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanTerrain.h"

#include "PacmanNavigationSpawnerComponent.h"
#include "PacmanPickupSpawnerComponent.h"


// Sets default values
APacmanTerrain::APacmanTerrain()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	
	NavigationSpawner = CreateDefaultSubobject<UPacmanNavigationSpawnerComponent>("NavigationSpawner");
	PickupSpawner = CreateDefaultSubobject <UPacmanPickupSpawnerComponent>("PickupSpawner");
}

void APacmanTerrain::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (WorldMap)
	{
		// We are only concerned on top mip map level
		auto &Mip = WorldMap->PlatformData->Mips[0];
		const FColor *Pixels = reinterpret_cast<const FColor*>(Mip.BulkData.LockReadOnly());

		FVector MeshMin;
		FVector MeshMax;
		Mesh->GetLocalBounds(MeshMin, MeshMax);

		FTransform ToWorld = ActorToWorld();
		MeshMin = ToWorld.TransformPosition(MeshMin);
		MeshMax = ToWorld.TransformPosition(MeshMax);

		FVector WorldStartLocation = MeshMin;
		FVector WorldSize = MeshMax - MeshMin;

		FVector PixelToWorld = FVector(WorldSize.X / static_cast<float>(Mip.SizeX), WorldSize.Y / static_cast<float>(Mip.SizeY), 1.f);

		float StartPixel = static_cast<float>((BlockSize / 2) + 1);
		
		// How many pixels we should skip to another block
		float Step = static_cast<float>(BlockSize) + 1.85f;

		PickupSpawner->Spawn(Pixels, Mip.SizeX, Mip.SizeY, StartPixel, Step, PixelToWorld, WorldStartLocation);
		NavigationSpawner->Spawn(Pixels, Mip.SizeX, Mip.SizeY, StartPixel, Step, PixelToWorld, WorldStartLocation);

		Mip.BulkData.Unlock();
	}
}

#if WITH_EDITOR
void APacmanTerrain::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty->GetName() == GET_MEMBER_NAME_STRING_CHECKED(APacmanTerrain, WorldMap))
	{
		// We are only concerned on top mip map level
		auto &Mip = WorldMap->PlatformData->Mips[0];
		const FColor *Pixels = reinterpret_cast<const FColor*>(Mip.BulkData.LockReadOnly());

		FindBlockSize(Pixels, Mip.SizeX, Mip.SizeY);

		Mip.BulkData.Unlock();
	}
}
void APacmanTerrain::FindBlockSize(const FColor * Pixels, int32 MapWidth, int32 MapHeight)
{
	// Path is the most narrowest part in the map
	BlockSize = MapWidth;

	// Counts how wide is path
	int32 Counter = 0;

	for (int32 i = 0; i < MapHeight; ++i)
	{
		for (int32 j = 0; j < MapWidth; ++j)
		{
			// White color symbolizes path
			if (Pixels[i * MapWidth + j].R == 255)
			{
				++Counter;
			}
			else
			{
				BlockSize = Counter < BlockSize && Counter > 0 ? Counter : BlockSize;
				Counter = 0;
			}
		}
	}
}
#endif



// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "../NavigationSystem/PacmanNavigationSystem.h"

#include "Components/ActorComponent.h"
#include "PacmanNavigationSpawnerComponent.generated.h"



/// PacmanNavigationSpawnerComponent is responsible for spawning navigation points
/// and assigning them to navigation system, so they can be used for path finding
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PACMAN_API UPacmanNavigationSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/// Sets default values for this component's properties
	UPacmanNavigationSpawnerComponent();

	/// Navigation nodes are spawned in every place where we can change direction
	void Spawn(const FColor *Pixels, int32 SizeX, int32 SizeY, float StartPixel, float Step, FVector PixelToWorld, FVector WorldStart);
	
private:
	/// We need root node(starting point) so we can started to building paths
	PacmanNavigationNode CreateRootNode(const FColor *Pixels, int32 SizeX, int32 SizeY, float StartPixel, float Step, FVector PixelToWorld, FVector WorldStart);
	
	/// If we have found a point on the map, we don't know, whether the node haven't already been spawned
	/// This method will find out and if node exists then it will link it with current node
	/// True means that node have already been spawned
	bool TryToLinkNodes(TArray<struct NavNodeConstructHelper> &OpenList, struct NavNodeConstructHelper &CurrentHelperNode, FVector NewHelperNodeTexCoords, int32 DirToBlock, float NodeRadiusInPixels);

	FPacmanNavigationData NavigationData;
};

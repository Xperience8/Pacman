// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanNavigationSpawnerComponent.h"

// Sets default values for this component's properties
UPacmanNavigationSpawnerComponent::UPacmanNavigationSpawnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

struct NavNodeConstructHelper
{
	// Node which we are constructing
	PacmanNavigationNode *Node;

	// Relative location to world map in pixels
	float TexCoordX;
	float TexCoordY;

	// Every node has connection only in for directions
	// If we linked two nodes, then they have blocked directions to each other(infinite loop protection)
	// For Example if first node is on the left and second node is on the right and we link them
	// then first node has blocked connection to the right and second node has blocked connection to the left
	bool DirectionsToLook[4];

	NavNodeConstructHelper(PacmanNavigationNode *InNode, float InTexCoordX, float InTexCoordY) : Node(InNode), TexCoordX(InTexCoordX), TexCoordY(InTexCoordY)
	{
		DirectionsToLook[0] = true;
		DirectionsToLook[1] = true;
		DirectionsToLook[2] = true;
		DirectionsToLook[3] = true;
	}
};

void UPacmanNavigationSpawnerComponent::Spawn(const FColor *Pixels, int32 SizeX, int32 SizeY, float StartPixel, float Step, FVector PixelToWorld, FVector WorldStart)
{
	NavigationData.Init();
	NavigationData.NavNodes.Add(CreateRootNode(Pixels, SizeX, SizeY, StartPixel, Step, PixelToWorld, WorldStart));

	// OpenList is collection of helper nodes, which were discovered but were not processed
	TArray<NavNodeConstructHelper> OpenList;

	FVector TexCoords = (NavigationData.NavNodes.Last().GetLocation() - WorldStart) / PixelToWorld;
	OpenList.Add(NavNodeConstructHelper(&NavigationData.NavNodes.Last(), TexCoords.X, TexCoords.Y));

	const FVector OffsetsToMoveInPixels[4] = { FVector(-Step, 0.f, 0.f), FVector(Step, 0.f, 0.f), FVector(0.f, Step, 0.f), FVector(0.f, -Step, 0.f) };

	while (OpenList.Num() > 0)
	{
		auto CurrentNodeHelper = OpenList.Pop();

		// Check every side for connection
		for (int32 i = 0; i < 4; ++i)
		{
			// If connection is blocked, then its mean that this connection already exists
			if (!CurrentNodeHelper.DirectionsToLook[i])
			{
				continue;
			}

			// Go in the specific direction until you find a place where node can be spawned
			FVector NewNodeTexCoords = FVector(CurrentNodeHelper.TexCoordX, CurrentNodeHelper.TexCoordY, 0.f) + OffsetsToMoveInPixels[i];
			while (Pixels[static_cast<int32>(NewNodeTexCoords.Y) * SizeX + static_cast<int32>(NewNodeTexCoords.X)].R == 255)
			{
				// Every place is considered as navigation node if we can change direction in it
				FVector ChangeDir1 = FRotator(0.f, 90.f, 0.f).RotateVector(OffsetsToMoveInPixels[i]);
				FVector ChangeDir2 = -ChangeDir1;

				bool CanChangeDir1 = Pixels[static_cast<int32>(NewNodeTexCoords.Y + ChangeDir1.Y) * SizeX + static_cast<int32>(NewNodeTexCoords.X + ChangeDir1.X)].R == 255;
				bool CanChangeDir2 = Pixels[static_cast<int32>(NewNodeTexCoords.Y + ChangeDir2.Y) * SizeX + static_cast<int32>(NewNodeTexCoords.X + ChangeDir2.X)].R == 255;
				if (CanChangeDir1 || CanChangeDir2)
				{
					// 0 => 1		1 => 0		2 => 3		3 => 2
					int32 DirToBlock = i % 2 == 0 ? i + 1 : i - 1;

					// We find a location, but we don't know whether node has already been spawned here
					if (!TryToLinkNodes(OpenList, CurrentNodeHelper, NewNodeTexCoords, DirToBlock, Step))
					{
						// It's new node
						NavigationData.NavNodes.Add(PacmanNavigationNode(NewNodeTexCoords *PixelToWorld + WorldStart));
						auto &LastNode = NavigationData.NavNodes.Last();

						NavigationData.Connect(*CurrentNodeHelper.Node, LastNode);

						// Add new node so every direction of this node can be traversed, except incoming direction
						OpenList.Add(NavNodeConstructHelper(&LastNode, NewNodeTexCoords.X, NewNodeTexCoords.Y));
						OpenList.Last().DirectionsToLook[DirToBlock] = false;
					}

					break;
				}

				NewNodeTexCoords += OffsetsToMoveInPixels[i];
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Nodes: %d"), NavigationData.NavNodes.Num());
	GNavigationSystem->SetNavigationData(&NavigationData);
}

PacmanNavigationNode UPacmanNavigationSpawnerComponent::CreateRootNode(const FColor *Pixels, int32 SizeX, int32 SizeY, float StartPixel, float Step, FVector PixelToWorld, FVector WorldStart)
{
	FVector Location;
	for (float Y = StartPixel; Y < static_cast<float>(SizeY); Y += Step)
	{
		for (float X = StartPixel; X < static_cast<float>(SizeX); X += Step)
		{
			// First white place on the world map is start location
			if (Pixels[static_cast<int32>(Y)* SizeX + static_cast<int32>(X)].R == 255)
			{
				Location = FVector(X, Y, 0.f) * PixelToWorld + WorldStart;
			}
		}
	}
	return PacmanNavigationNode(Location);
}

bool UPacmanNavigationSpawnerComponent::TryToLinkNodes(TArray<NavNodeConstructHelper> &OpenList, NavNodeConstructHelper &CurrentHelperNode, FVector NewHelperNodeTexCoords, int32 DirToBlock, float NodeRadiusInPixels)
{
	bool SuccessfullyLinked = false;

	// Find out whether node was already created
	for (auto &HelperNode : OpenList)
	{
		FVector Offset = NewHelperNodeTexCoords - FVector(HelperNode.TexCoordX, HelperNode.TexCoordY, 0.f);
		
		// It's too close -> requested node exists
		if (Offset.Size() < NodeRadiusInPixels)
		{
			NavigationData.Connect(*CurrentHelperNode.Node, *HelperNode.Node);

			HelperNode.DirectionsToLook[DirToBlock] = false;
			SuccessfullyLinked = true;
			break;
		}
	}

	return SuccessfullyLinked;
}

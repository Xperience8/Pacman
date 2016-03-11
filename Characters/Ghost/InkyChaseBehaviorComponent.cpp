// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "InkyChaseBehaviorComponent.h"

#include "PacmanPathFollowingComponent.h"

#include "Ghost.h"
#include "../Player/PacmanPlayer.h"

void UInkyChaseBehaviorComponent::FindPath()
{
	auto Owner = static_cast<AGhost*>(GetOwner());
	auto PathFollower = Owner->GetPathFollowingComponent();

	TActorIterator<APacmanPlayer> PlayerIt(GetWorld());
	auto PlayerNode = GNavigationSystem->GetNearestNode(PlayerIt->GetActorLocation());

	FVector TargetLocation = PlayerNode->GetLocation();
	for (TActorIterator<AGhost> GhostIt(GetWorld()); GhostIt; ++GhostIt)
	{
		if (GhostIt->IsBlinky())
		{
			TargetLocation = (PlayerNode->GetLocation() - GhostIt->GetActorLocation()) * 2.f;
			break;
		}
	}

	auto TargetNode = GNavigationSystem->GetNearestNodeRoughly(TargetLocation);

	GNavigationSystem->FindPathToLocation(Owner->GetActorLocation(), TargetNode->GetLocation(), Owner->GetActorForwardVector(), PathFollower->GetPath());
	PathFollower->StartPathFollowing();
}
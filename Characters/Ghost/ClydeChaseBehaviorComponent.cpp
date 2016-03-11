// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "ClydeChaseBehaviorComponent.h"

#include "PacmanPathFollowingComponent.h"

#include "Ghost.h"
#include "../Player/PacmanPlayer.h"

void UClydeChaseBehaviorComponent::FindPath()
{
	auto Owner = static_cast<AGhost*>(GetOwner());
	auto PathFollower = Owner->GetPathFollowingComponent();

	TActorIterator<APacmanPlayer> PlayerIt(GetWorld());
	float DistanceToPlayer = (PlayerIt->GetActorLocation() - Owner->GetActorLocation()).Size2D();

	FVector TargetLocation = Owner->GetHomeLocation();
	if (DistanceToPlayer < MinDistanceToStartChasing)
	{
		TargetLocation = PlayerIt->GetActorLocation();
	}

	GNavigationSystem->FindPathToLocation(Owner->GetActorLocation(), TargetLocation, Owner->GetActorForwardVector(), PathFollower->GetPath());
	PathFollower->StartPathFollowing();
}



// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "BlinkyChaseBehaviorComponent.h"

#include "PacmanPathFollowingComponent.h"

#include "Ghost.h"
#include "../Player/PacmanPlayer.h"

void UBlinkyChaseBehaviorComponent::FindPath()
{
	auto Owner = static_cast<AGhost*>(GetOwner());
	auto PathFollower = Owner->GetPathFollowingComponent();

	TActorIterator<APacmanPlayer> PlayerIt(GetWorld());

	GNavigationSystem->FindPathToLocation(Owner->GetActorLocation(), PlayerIt->GetActorLocation(), Owner->GetActorForwardVector(), PathFollower->GetPath());
	PathFollower->StartPathFollowing();
}

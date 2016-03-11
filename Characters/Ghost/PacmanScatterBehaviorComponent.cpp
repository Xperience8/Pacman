// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanScatterBehaviorComponent.h"

#include "Ghost.h"
#include "PacmanPathFollowingComponent.h"

void UPacmanScatterBehaviorComponent::FindPath()
{
	auto Owner = static_cast<AGhost*>(GetOwner());
	auto PathFollower = Owner->GetPathFollowingComponent();

	GNavigationSystem->FindPathToLocation(Owner->GetActorLocation(), Owner->GetHomeLocation(), Owner->GetActorForwardVector(), PathFollower->GetPath());
	PathFollower->StartPathFollowing();
}

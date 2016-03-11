// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PinkyChaseBehaviorComponent.h"

#include "PacmanPathFollowingComponent.h"

#include "Ghost.h"
#include "../Player/PacmanPlayer.h"

void UPinkyChaseBehaviorComponent::FindPath()
{
	auto Owner = static_cast<AGhost*>(GetOwner());
	auto PathFollower = Owner->GetPathFollowingComponent();

	TActorIterator<APacmanPlayer> PlayerIt(GetWorld());

	auto PlayerNode = GNavigationSystem->GetNearestNode(PlayerIt->GetActorLocation());
	auto TargetNode = PlayerNode;

	for (auto &Connection : PlayerNode->GetConnections())
	{
		FVector DirToNode = (Connection.GetTail()->GetLocation() - PlayerIt->GetActorLocation()).GetUnsafeNormal();
		if (FVector::DotProduct(DirToNode, PlayerIt->GetActorForwardVector()) > 0.5f)
		{
			TargetNode = Connection.GetTail();
			break;
		}
	}

	GNavigationSystem->FindPathToLocation(Owner->GetActorLocation(), TargetNode->GetLocation(), Owner->GetActorForwardVector(), PathFollower->GetPath());
	PathFollower->StartPathFollowing();
}


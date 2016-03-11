// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanFearedBehaviorComponent.h"

#include "Ghost.h"
#include "PacmanPathFollowingComponent.h"

#include "../Player/PacmanPlayer.h"

void UPacmanFearedBehaviorComponent::ActivateBehavior()
{
	Super::ActivateBehavior();

	auto Owner = static_cast<AGhost*>(GetOwner());
	auto PathFollower = Owner->GetPathFollowingComponent();

	auto CurrentSegment = GNavigationSystem->GetNearestNode(Owner->GetActorLocation());
	
	auto &Path = PathFollower->GetPath();
	Path.Destroy();
	Path.PostInit(CurrentSegment);

	PathFollower->StartPathFollowing();
}

void UPacmanFearedBehaviorComponent::OnPathFinished()
{
	TActorIterator<APacmanPlayer> PlayerIt(GetWorld());

	auto Owner = static_cast<AGhost*>(GetOwner());
	auto PathFollower = Owner->GetPathFollowingComponent();
	auto &Path = PathFollower->GetPath();

	auto CurrentSegment = Path.GetGoalSegment();

	PacmanNavigationNode *NewGoalSegmentPrimary = nullptr;
	PacmanNavigationNode *NewGoalSegmentSecondary = nullptr;

	for (auto &Connection : CurrentSegment->GetConnections())
	{
		FVector GhostOffsetFromNewNode = Connection.GetTail()->GetLocation() - Owner->GetActorLocation();
		FVector PlayerOffsetFromGhost = PlayerIt->GetActorLocation() - Owner->GetActorLocation();

		// Don't go backward
		if (FVector::DotProduct(GhostOffsetFromNewNode.GetUnsafeNormal(), Owner->GetActorForwardVector()) <= -0.75f)
		{
			continue;
		}

		NewGoalSegmentSecondary = Connection.GetTail();

		// Don't go directly to the player
		if (FVector::DotProduct(GhostOffsetFromNewNode.GetUnsafeNormal(), PlayerOffsetFromGhost.GetUnsafeNormal()) >= 0.25f)
		{
			continue;
		}

		NewGoalSegmentPrimary = Connection.GetTail();
		break;
	}

	if (!NewGoalSegmentPrimary)
	{
		NewGoalSegmentPrimary = NewGoalSegmentSecondary;
	}


	Path.Destroy();
	Path.PostInit(NewGoalSegmentPrimary);
	PathFollower->StartPathFollowing();
}

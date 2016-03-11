// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanPathFollowingComponent.h"

#include "Ghost.h"
#include "PacmanBehaviorComponent.h"

// Sets default values for this component's properties
UPacmanPathFollowingComponent::UPacmanPathFollowingComponent() : bFollowPath(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called every frame
void UPacmanPathFollowingComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	if (bFollowPath)
	{
		AGhost *Owner = static_cast<AGhost*>(GetOwner());

		// Get actor location in 2d space(up is ignored)	
		FVector Location = Owner->GetActorLocation();
		Location.Z = 0.f;

		auto CurrentSegment = Path.GetCurrentSegment();
		FVector MovementDir = (CurrentSegment->GetLocation() - Location).GetUnsafeNormal();

		Owner->GetMovementComponent()->AddInputVector(MovementDir);

		if (Path.CheckForNextSegment(Location))
		{
			Owner->GetActiveBehavior()->OnSegmentFinished();
		}

		if (Path.IsAtGoal())
		{
			bFollowPath = false;
			Owner->GetActiveBehavior()->OnPathFinished();
		}
	}
}

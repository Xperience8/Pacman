// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Characters/Ghost/PacmanBehaviorComponent.h"
#include "PinkyChaseBehaviorComponent.generated.h"

/// Pinky always goes to node before player
UCLASS()
class PACMAN_API UPinkyChaseBehaviorComponent : public UPacmanBehaviorComponent
{
	GENERATED_BODY()
	
public:

	virtual void ActivateBehavior() override
	{
		Super::ActivateBehavior();
		FindPath();
	}
	virtual void OnSegmentFinished() override
	{
		FindPath();
	}
	virtual void OnPathFinished() override
	{
		FindPath();
	}

private:
	void FindPath();
};

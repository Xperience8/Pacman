// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Characters/Ghost/PacmanBehaviorComponent.h"
#include "InkyChaseBehaviorComponent.generated.h"

/// Inky follows specific locations which is computed:
/// 1.) If there is blinky
///		a) Find vector from blinky to player current location
///      b) Double found vector and add it to blinky location
///		c) This is inky's new target location
/// 2) If there isn't blinky then follows directly player(new blinky)
UCLASS()
class PACMAN_API UInkyChaseBehaviorComponent : public UPacmanBehaviorComponent
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

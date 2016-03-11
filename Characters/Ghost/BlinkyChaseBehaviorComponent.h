// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Characters/Ghost/PacmanBehaviorComponent.h"
#include "BlinkyChaseBehaviorComponent.generated.h"


/// Blinky always follows player's actual location
UCLASS()
class PACMAN_API UBlinkyChaseBehaviorComponent : public UPacmanBehaviorComponent
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

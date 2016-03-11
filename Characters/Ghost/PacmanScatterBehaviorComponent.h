// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Characters/Ghost/PacmanBehaviorComponent.h"
#include "PacmanScatterBehaviorComponent.generated.h"

UCLASS()
class PACMAN_API UPacmanScatterBehaviorComponent : public UPacmanBehaviorComponent
{
	GENERATED_BODY()
	
public:

	virtual void ActivateBehavior() override
	{
		Super::ActivateBehavior();
		FindPath();
	}
	virtual void OnPathFinished() override 
	{ 
		FindPath(); 
	}

private:
	void FindPath();
};

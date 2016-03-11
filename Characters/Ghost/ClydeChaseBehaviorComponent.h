// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Characters/Ghost/PacmanBehaviorComponent.h"
#include "ClydeChaseBehaviorComponent.generated.h"

/// Clyde chases player only if he is nearby
UCLASS()
class PACMAN_API UClydeChaseBehaviorComponent : public UPacmanBehaviorComponent
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

	UPROPERTY(EditAnywhere)
	float MinDistanceToStartChasing;
};

// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Characters/Ghost/PacmanBehaviorComponent.h"
#include "PacmanFearedBehaviorComponent.generated.h"

/// When player is hunter then this behavior is activated
/// Ghost tries to avoid him
UCLASS()
class PACMAN_API UPacmanFearedBehaviorComponent : public UPacmanBehaviorComponent
{
	GENERATED_BODY()
	
public:

	virtual void ActivateBehavior() override;
	virtual void OnPathFinished() override;
};

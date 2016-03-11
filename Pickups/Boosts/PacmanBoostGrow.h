// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Pickups/PacmanPickup.h"
#include "PacmanBoostGrow.generated.h"

/// PacmanBoostGrow is a pickup that can scale player for limited time
UCLASS()
class PACMAN_API APacmanBoostGrow : public APacmanPickup
{
	GENERATED_BODY()
	
private:
	/// Grow player and add him ability to eat ghosts for limited time
	virtual void Pickup(AActor *Player) final override;
};

// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Pickups/PacmanPickup.h"
#include "PacmanBoostShooting.generated.h"

/// PacmanBoostShooting is a pickup that can activate shooting for limited time
UCLASS()
class PACMAN_API APacmanBoostShooting : public APacmanPickup
{
	GENERATED_BODY()
	
private:
	/// Add shooting ability to the player(only for limited time)
	virtual void Pickup(AActor *Player) final override;
};

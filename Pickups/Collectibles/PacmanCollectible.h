// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Pickups/PacmanPickup.h"
#include "PacmanCollectible.generated.h"

/// PacmanCollectible is a pickup that add points to the player when it is picked up
UCLASS()
class PACMAN_API APacmanCollectible : public APacmanPickup
{
	GENERATED_BODY()
	
private:
	/// Add points to the player
	virtual void Pickup(AActor *Player) final override;

	/// How many points are added to the player
	UPROPERTY(EditDefaultsOnly)
		int32 NumPoints;
};

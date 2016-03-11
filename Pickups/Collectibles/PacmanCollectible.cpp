// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanCollectible.h"

#include "../../Characters/Player/PacmanPlayerController.h"

void APacmanCollectible::Pickup(AActor *Player)
{
	// Other actor can not take pickup, therefore this cast is safe
	static_cast<APacmanPlayerController*>(static_cast<APawn*>(Player)->GetController())->AddPoints(NumPoints);
}



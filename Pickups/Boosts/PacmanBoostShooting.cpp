// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanBoostShooting.h"

#include "../../Characters/Player/PacmanPlayer.h"
#include "../../Characters/Player/PacmanShootingComponent.h"
void APacmanBoostShooting::Pickup(AActor *Player)
{
	// Other actor can not take pickup, therefore this cast is safe
	static_cast<APacmanPlayer*>(Player)->GetShootingComponent()->EnableShooting();
}



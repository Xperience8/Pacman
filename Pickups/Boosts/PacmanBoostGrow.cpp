// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanBoostGrow.h"

#include "../../Characters/Player/PacmanPlayer.h"
#include "../../Characters/Player/PacmanScalingComponent.h"

void APacmanBoostGrow::Pickup(AActor *Player)
{
	// Other actor can not take pickup, therefore this cast is safe
	static_cast<APacmanPlayer*>(Player)->GetScalingComponent()->Scale();
}




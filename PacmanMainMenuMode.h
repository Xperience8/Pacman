// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/GameMode.h"
#include "PacmanMainMenuMode.generated.h"

/// PacmanMainMenuMode is responsible only for setting default class(HUD and controller)
UCLASS()
class PACMAN_API APacmanMainMenuMode : public AGameMode
{
	GENERATED_BODY()
public:
		APacmanMainMenuMode();
	
	
};

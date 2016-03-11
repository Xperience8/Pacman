// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanMainMenuMode.h"

#include "UI/PacmanMainMenuHUD.h"
#include "MainMenuController.h"

APacmanMainMenuMode::APacmanMainMenuMode()
{
	HUDClass = APacmanMainMenuHUD::StaticClass();
	PlayerControllerClass = AMainMenuController::StaticClass();
}



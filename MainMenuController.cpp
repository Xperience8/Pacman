// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "MainMenuController.h"
#include "UI/PacmanMainMenuHUD.h"

void AMainMenuController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	bShowMouseCursor = true;
}
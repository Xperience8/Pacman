// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanGameInstance.h"

#include "NavigationSystem/PacmanNavigationSystem.h"

void UPacmanGameInstance::Init()
{
	NavigationSystem = NewObject<UPacmanNavigationSystem>(UPacmanNavigationSystem::StaticClass());
	GNavigationSystem = NavigationSystem;

	Super::Init();
}


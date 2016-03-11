// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanGameMode.h"

#include "UI/PacmanGameHUD.h"
#include "Characters/Player/PacmanPlayerController.h"

#include "Terrain/PacmanTerrain.h"
#include "Terrain/PacmanPickupSpawnerComponent.h"

APacmanGameMode::APacmanGameMode() : CollectiblesLeft(0)
{
	ConstructorHelpers::FClassFinder<APawn> PlayerPawn(TEXT("/Game/Blueprints/PlayerTemplate"));

	HUDClass = APacmanGameHUD::StaticClass();
	DefaultPawnClass = PlayerPawn.Class;
	PlayerControllerClass = APacmanPlayerController::StaticClass();
}

void APacmanGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TActorIterator<APacmanTerrain> PacmanTerrainIt(GetWorld());
	CollectiblesLeft = PacmanTerrainIt->GetPickupSpawner()->GetNumCollectibles();	
}

void APacmanGameMode::CollectibleFound()
{
	if (--CollectiblesLeft == 0)
	{
		static_cast<APacmanPlayerController*>(GetWorld()->GetFirstPlayerController())->ShowLevelSummary(true);
	}
}

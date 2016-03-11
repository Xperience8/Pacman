// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/GameMode.h"
#include "PacmanGameMode.generated.h"

/// PacmanGameMode is responsible for handling collectibles
/// It there aren't any collectibles then level is completed
UCLASS()
class PACMAN_API APacmanGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	APacmanGameMode();

	/// Find out how many collectibles are needed to complete level
	virtual void PostInitializeComponents() override;

	/// Called when player picks up collectible
	/// When all collectibles are found then level is completed
	void CollectibleFound();

private:
	/// How many collectibles are left in level
	/// If it drops to zero then level is completed
	int32 CollectiblesLeft;
};

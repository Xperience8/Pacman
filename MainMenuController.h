// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MainMenuController.generated.h"

/// MainMenuController is only responsible to show mouse cursor
UCLASS()
class PACMAN_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()

	virtual void PostInitializeComponents() final override;
	
};

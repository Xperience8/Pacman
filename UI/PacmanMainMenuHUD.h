// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/HUD.h"
#include "PacmanMainMenuHUD.generated.h"

/// This class is responsible for drawing main menu
/// and setting controls to UI mode(cursor is visible)
UCLASS()
class PACMAN_API APacmanMainMenuHUD : public AHUD
{
	GENERATED_BODY()
	
	virtual void PostInitializeComponents() final override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final override;
public:
	void DisplayMenu(TSharedPtr<class SCompoundWidget> MenuToDisplay);
	void HideMenu();

private:
	TArray<TSharedPtr<class SCompoundWidget> > DisplayedMenuStack;

	TSharedPtr<class SPacmanMainMenuWidget> MainMenuWidget;
};

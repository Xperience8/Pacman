// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanMainMenuHUD.h"
#include "Widgets/SPacmanMainMenuWidget.h"

void APacmanMainMenuHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SAssignNew(MainMenuWidget, SPacmanMainMenuWidget).HUD(this);
	
	DisplayMenu(MainMenuWidget.ToSharedRef());
}

void APacmanMainMenuHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	while (DisplayedMenuStack.Num() > 2)
	{
		HideMenu();
	}

	GEngine->GameViewport->RemoveViewportWidgetContent(MainMenuWidget.ToSharedRef());
	MainMenuWidget.Reset();
}

void APacmanMainMenuHUD::DisplayMenu(TSharedPtr<SCompoundWidget> MenuToDisplay)
{
	// If there is visible menu, hide it
	if (DisplayedMenuStack.Num() > 0)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(DisplayedMenuStack.Top().ToSharedRef());
	}

	// Show new menu
	GEngine->GameViewport->AddViewportWidgetContent(MenuToDisplay.ToSharedRef());
	DisplayedMenuStack.Push(MenuToDisplay.ToSharedRef());

	// Only GUI elements can handle input
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewport(true);
	InputMode.SetWidgetToFocus(MenuToDisplay);
	GetOwningPlayerController()->SetInputMode(InputMode);
}

void APacmanMainMenuHUD::HideMenu()
{
	// There must be at least 2 menus, main menu and one sub-menu that we want to hide
	if (DisplayedMenuStack.Num() < 2)
	{
		return;
	}

	GEngine->GameViewport->RemoveViewportWidgetContent(DisplayedMenuStack.Pop().ToSharedRef());

	auto &MenuToDisplay = DisplayedMenuStack.Top();

	// Show previous menu
	GEngine->GameViewport->AddViewportWidgetContent(MenuToDisplay.ToSharedRef());

	// Only GUI elements can handle input
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewport(true);
	InputMode.SetWidgetToFocus(MenuToDisplay);
	GetOwningPlayerController()->SetInputMode(InputMode);
}

// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanGameHUD.h"

#include "Widgets/SPacmanGameWidget.h"
#include "Widgets/SPacmanLevelSummaryWidget.h"
#include "Widgets/SPacmanRespawnWidget.h"

void APacmanGameHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SAssignNew(GameWidget, SPacmanGameWidget);
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(GameWidget.ToSharedRef()));

	FInputModeGameOnly InputMode;
	GetOwningPlayerController()->SetInputMode(InputMode);
}

void APacmanGameHUD::ShowLevelSummary(bool PlayerWins)
{
	SAssignNew(LevelSummaryWidget, SPacmanLevelSummaryWidget).PlayerWins(PlayerWins).HUD(this);
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(LevelSummaryWidget.ToSharedRef()));

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewport(true);
	InputMode.SetWidgetToFocus(LevelSummaryWidget);
	GetOwningPlayerController()->SetInputMode(InputMode);
}

void APacmanGameHUD::ShowRespawnScreen(int32 TimeToRespawnPlayer)
{
	SAssignNew(RespawnWidget, SPacmanRespawnWidget);
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(RespawnWidget.ToSharedRef()));
	
	TimeToRespawn = TimeToRespawnPlayer;
	RespawnWidget->SetRespawnTime(TimeToRespawn);

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &APacmanGameHUD::RespawnCountdown, 1.f, true);
}
void APacmanGameHUD::HideRespawnScreen()
{
	GetWorldTimerManager().ClearTimer(RespawnTimerHandle);

	GEngine->GameViewport->RemoveViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(RespawnWidget.ToSharedRef()));
	RespawnWidget.Reset();
}
void APacmanGameHUD::RespawnCountdown()
{
	RespawnWidget->SetRespawnTime(--TimeToRespawn);
}
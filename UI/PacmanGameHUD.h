// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/HUD.h"
#include "PacmanGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API APacmanGameHUD : public AHUD
{
	GENERATED_BODY()
	
	virtual void PostInitializeComponents() final override;

public:
	
	void ShowLevelSummary(bool PlayerWins);
	
	void ShowRespawnScreen(int32 TimeToRespawn);
	void HideRespawnScreen();


	const TSharedPtr<class SPacmanGameWidget> &GetGameWidget() const
	{
		return GameWidget;
	}


private:
	void RespawnCountdown();

	TSharedPtr<class SPacmanGameWidget> GameWidget;
	TSharedPtr<class SPacmanLevelSummaryWidget> LevelSummaryWidget;
	
	TSharedPtr<class SPacmanRespawnWidget> RespawnWidget;
	FTimerHandle RespawnTimerHandle;
	int32 TimeToRespawn;
};

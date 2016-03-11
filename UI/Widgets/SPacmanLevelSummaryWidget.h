// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once


#include "Widgets/SCompoundWidget.h"

/// This class is responsible for showing whether player successfully completes level or not
/// After player's click go to main menu
class PACMAN_API SPacmanLevelSummaryWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPacmanLevelSummaryWidget)
	{}
	SLATE_ARGUMENT(bool, PlayerWins)
	SLATE_ARGUMENT(class APacmanGameHUD*, HUD)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	FReply ReturnToMainMenu();

	class APacmanGameHUD *ParentHUD;
};

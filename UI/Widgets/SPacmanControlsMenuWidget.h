// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once


#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class PACMAN_API SPacmanControlsMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPacmanControlsMenuWidget)
	{}
	SLATE_ARGUMENT(class APacmanMainMenuHUD*, HUD)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
private:
	FReply ReturnClick();

	class APacmanMainMenuHUD *ParentHUD;
};

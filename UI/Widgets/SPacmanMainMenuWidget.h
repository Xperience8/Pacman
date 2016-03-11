// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once


#include "Widgets/SCompoundWidget.h"

class PACMAN_API SPacmanMainMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPacmanMainMenuWidget)
	{}
	SLATE_ARGUMENT(class APacmanMainMenuHUD*, HUD)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
private:
	FReply PlayClick();
	FReply OptionsClick();
	FReply ExitClick();
	
	class APacmanMainMenuHUD *ParentHUD;

	TSharedPtr<class SPacmanControlsMenuWidget> ControlsSubMenu;
};

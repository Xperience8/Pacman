// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once


#include "Widgets/SCompoundWidget.h"

/// This class is responsible for showing HUD data during game-play
class PACMAN_API SPacmanGameWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPacmanGameWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

public:
	/// Shows how many lives has player
	void SetNumLives(int32 NumLives);

	/// Shows how many points has player acquired
	void SetNumPoints(int32 NumPoints);

private:
	TSharedPtr<SHorizontalBox> PlayerLifesBox;
	TSharedPtr<STextBlock> PlayerScore;
	
	const struct FPacmanGameStyle *Style;
};

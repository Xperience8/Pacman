// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once


#include "Widgets/SCompoundWidget.h"

/// This widget is created when player dies
/// and it's purpose is to show him how many seconds are left to spawn
class PACMAN_API SPacmanRespawnWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPacmanRespawnWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/// Shows how many seconds left for player spawn
	void SetRespawnTime(int32 RespawnTime);
private:
	TSharedPtr<STextBlock> RespawnTimeText;
};

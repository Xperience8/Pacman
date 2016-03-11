// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanGameWidgetStyle.h"


FPacmanGameStyle::FPacmanGameStyle()
{
}

FPacmanGameStyle::~FPacmanGameStyle()
{
}

const FName FPacmanGameStyle::TypeName(TEXT("FPacmanGameStyle"));

const FPacmanGameStyle& FPacmanGameStyle::GetDefault()
{
	static FPacmanGameStyle Default;
	return Default;
}

void FPacmanGameStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}


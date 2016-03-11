// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanMenuWidgetStyle.h"


FPacmanMenuStyle::FPacmanMenuStyle()
{
}

FPacmanMenuStyle::~FPacmanMenuStyle()
{
}

const FName FPacmanMenuStyle::TypeName(TEXT("FPacmanMenuStyle"));

const FPacmanMenuStyle& FPacmanMenuStyle::GetDefault()
{
	static FPacmanMenuStyle Default;
	return Default;
}

void FPacmanMenuStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}


// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanReportWidgetStyle.h"


FPacmanReportStyle::FPacmanReportStyle()
{
}

FPacmanReportStyle::~FPacmanReportStyle()
{
}

const FName FPacmanReportStyle::TypeName(TEXT("FPacmanReportStyle"));

const FPacmanReportStyle& FPacmanReportStyle::GetDefault()
{
	static FPacmanReportStyle Default;
	return Default;
}

void FPacmanReportStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}


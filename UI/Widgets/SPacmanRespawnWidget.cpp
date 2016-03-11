// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "SPacmanRespawnWidget.h"
#include "SlateOptMacros.h"
#include "../Styles/PacmanStyles.h"
#include "../Styles/PacmanReportWidgetStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SPacmanRespawnWidget::Construct(const FArguments& InArgs)
{
	auto &Style = FPacmanStyles::Get()->GetWidgetStyle<FPacmanReportStyle>("ReportStyle");

	ChildSlot
	[
		// Populate the widget
		SNew(SOverlay)
		+ SOverlay::Slot()
		.VAlign(EVerticalAlignment::VAlign_Fill)
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			SNew(SColorBlock)
			.Color(FColor(0, 0, 0, 127))
		]
		+ SOverlay::Slot()
		.VAlign(EVerticalAlignment::VAlign_Top)
		.HAlign(EHorizontalAlignment::HAlign_Center)
		[
			SNew(STextBlock)
			.TextStyle(&Style.Title)
			.Text(FText::FromString("You have died"))
		]
		+ SOverlay::Slot()
		.VAlign(EVerticalAlignment::VAlign_Bottom)
		.HAlign(EHorizontalAlignment::HAlign_Center)
		[
			SAssignNew(RespawnTimeText, STextBlock)
			.TextStyle(&Style.Message)
			.Text(FText::FromString("Respawn in: "))
		]	
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SPacmanRespawnWidget::SetRespawnTime(int32 RespawnTime)
{
	RespawnTimeText->SetText(FText::FromString("Respawn in: " + FString::FromInt(RespawnTime)));
}
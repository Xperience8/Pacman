// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "SPacmanLevelSummaryWidget.h"
#include "SlateOptMacros.h"

#include "../Styles/PacmanStyles.h"
#include "../Styles/PacmanReportWidgetStyle.h"

#include "../PacmanGameHUD.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SPacmanLevelSummaryWidget::Construct(const FArguments& InArgs)
{
	ParentHUD = InArgs._HUD;
	auto Style = &FPacmanStyles::Get()->GetWidgetStyle<FPacmanReportStyle>("ReportStyle");

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
			.TextStyle(&Style->Title)
			.Text(FText::FromString(InArgs._PlayerWins ? "You have won" : "You have failed"))
		]
		+ SOverlay::Slot()
		.VAlign(EVerticalAlignment::VAlign_Bottom)
		.HAlign(EHorizontalAlignment::HAlign_Center)
		[
			SNew(STextBlock)
			.TextStyle(&Style->Message)
			.Text(FText::FromString("Click to continue"))
		]
		+ SOverlay::Slot()
		.VAlign(EVerticalAlignment::VAlign_Fill)
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			SNew(SButton)
			.ButtonStyle(&Style->ClickableArea)
			.OnClicked(this, &SPacmanLevelSummaryWidget::ReturnToMainMenu)
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SPacmanLevelSummaryWidget::ReturnToMainMenu()
{
	ParentHUD->GetWorld()->GetFirstPlayerController()->ClientReturnToMainMenu("End");

	return FReply::Handled();
}
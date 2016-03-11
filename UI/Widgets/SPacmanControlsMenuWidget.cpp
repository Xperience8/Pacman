// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "SPacmanControlsMenuWidget.h"
#include "SlateOptMacros.h"

#include "../Styles/PacmanStyles.h"
#include "../Styles/PacmanMenuWidgetStyle.h"

#include "../PacmanMainMenuHUD.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SPacmanControlsMenuWidget::Construct(const FArguments& InArgs)
{
	ParentHUD = InArgs._HUD;

	auto &MenuStyle = FPacmanStyles::Get()->GetWidgetStyle<FPacmanMenuStyle>("MenuStyle");

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.VAlign(EVerticalAlignment::VAlign_Top)
		.HAlign(EHorizontalAlignment::HAlign_Left)
		[
			SNew(SImage)
			.Image(&MenuStyle.BackgroundImage)
		]

		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Right)
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			[
				SNew(SBox)
				.WidthOverride(FOptionalSize(800.f))
				[
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("Move Forward")))
						.TextStyle(&MenuStyle.Item)
					]
					+SVerticalBox::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("Move Backward")))
						.TextStyle(&MenuStyle.Item)
					]
					+SVerticalBox::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("Move Left")))
						.TextStyle(&MenuStyle.Item)
					]
					+SVerticalBox::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("Move Right")))
						.TextStyle(&MenuStyle.Item)
					]
					+SVerticalBox::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("Change Camera")))
						.TextStyle(&MenuStyle.Item)
					]
					+SVerticalBox::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("Shoot")))
						.TextStyle(&MenuStyle.Item)
					]
				]
			]
			+SHorizontalBox::Slot()
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("W")))
					.TextStyle(&MenuStyle.Item)
				]
				+SVerticalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("S")))
					.TextStyle(&MenuStyle.Item)
				]
				+SVerticalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("A")))
					.TextStyle(&MenuStyle.Item)
				]
				+SVerticalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("D")))
					.TextStyle(&MenuStyle.Item)
				]
				+SVerticalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("T")))
					.TextStyle(&MenuStyle.Item)
				]
				+SVerticalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("Backspace")))
					.TextStyle(&MenuStyle.Item)
				]
			]
		]
		+ SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Right)
		.VAlign(EVerticalAlignment::VAlign_Bottom)
		[
			SNew(SBox)
			.WidthOverride(FOptionalSize(1200.f))
			[
				SNew(SButton)
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.Text(FText::FromString(TEXT("Return")))
				.TextStyle(&MenuStyle.ButtonText)
				.ButtonStyle(&MenuStyle.Button)
				.OnClicked(this, &SPacmanControlsMenuWidget::ReturnClick)
			]
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SPacmanControlsMenuWidget::ReturnClick()
{
	ParentHUD->HideMenu();

	return FReply::Handled();
}
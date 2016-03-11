// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "SPacmanMainMenuWidget.h"
#include "SlateOptMacros.h"

#include "../Styles/PacmanStyles.h"
#include "../Styles/PacmanMenuWidgetStyle.h"

#include "../PacmanMainMenuHUD.h"

#include "SPacmanControlsMenuWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SPacmanMainMenuWidget::Construct(const FArguments& InArgs)
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

		+ SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Right)
		.VAlign(EVerticalAlignment::VAlign_Top)
		.Padding(FMargin(0.f, 250.f, 0.f, 0.f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(SBox)
				.WidthOverride(FOptionalSize(1200.f))
				[
					SNew(SButton)
					.HAlign(EHorizontalAlignment::HAlign_Center)
					.Text(FText::FromString(TEXT("Play")))
					.TextStyle(&MenuStyle.ButtonText)
					.ButtonStyle(&MenuStyle.Button)
					.OnClicked(this, &SPacmanMainMenuWidget::PlayClick)
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SBox)
				.WidthOverride(FOptionalSize(1200.f))
				[
					SNew(SButton)
					.HAlign(EHorizontalAlignment::HAlign_Center)
					.Text(FText::FromString(TEXT("Controls")))
					.TextStyle(&MenuStyle.ButtonText)
					.ButtonStyle(&MenuStyle.Button)
					.OnClicked(this, &SPacmanMainMenuWidget::OptionsClick)
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SBox)
				.WidthOverride(FOptionalSize(1200.f))
				[
					SNew(SButton)
					.HAlign(EHorizontalAlignment::HAlign_Center)
					.Text(FText::FromString(TEXT("Exit")))
					.TextStyle(&MenuStyle.ButtonText)
					.ButtonStyle(&MenuStyle.Button)
					.OnClicked(this, &SPacmanMainMenuWidget::ExitClick)
				]
			]
		]
	];

	SAssignNew(ControlsSubMenu, SPacmanControlsMenuWidget).HUD(ParentHUD);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SPacmanMainMenuWidget::PlayClick()
{
	UGameplayStatics::OpenLevel(ParentHUD, TEXT("/Game/Maps/Level1"));

	return FReply::Handled();
}

FReply SPacmanMainMenuWidget::OptionsClick()
{
	ParentHUD->DisplayMenu(ControlsSubMenu);

	return FReply::Handled();
}

FReply SPacmanMainMenuWidget::ExitClick()
{
	ParentHUD->GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");

	return FReply::Handled();
}


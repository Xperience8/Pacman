// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "SPacmanGameWidget.h"
#include "SlateOptMacros.h"

#include "../Styles/PacmanStyles.h"
#include "../Styles/PacmanGameWidgetStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SPacmanGameWidget::Construct(const FArguments& InArgs)
{
	Style = &FPacmanStyles::Get()->GetWidgetStyle<FPacmanGameStyle>("GameStyle");
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.VAlign(EVerticalAlignment::VAlign_Bottom)
		.HAlign(EHorizontalAlignment::HAlign_Right)
		[
			SAssignNew(PlayerScore, STextBlock)
			.TextStyle(&(Style->ScoreStyle))
			.Text(FText::FromString("Score: 0"))
		]
		+SOverlay::Slot()
		.VAlign(EVerticalAlignment::VAlign_Bottom)
		.HAlign(EHorizontalAlignment::HAlign_Left)
		[
			SAssignNew(PlayerLifesBox, SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(SImage)
				.Image(&(Style->LifeImage))
			]
			+ SHorizontalBox::Slot()
			[
				SNew(SImage)
				.Image(&(Style->LifeImage))
			]
			+ SHorizontalBox::Slot()
			[
				SNew(SImage)
				.Image(&(Style->LifeImage))
			]
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SPacmanGameWidget::SetNumLives(int32 NumLives)
{
	// If player has more lives than he has at the beginning
	if (PlayerLifesBox->NumSlots() < NumLives)
	{
		// How many lives we need to add
		int32 LivesToAdd = NumLives - PlayerLifesBox->NumSlots();
		for (int32 i = 0; i < LivesToAdd; ++i)
		{
			auto Image = SNew(SImage);
			Image->SetImage(&(Style->LifeImage));

			auto &ImageSlot = PlayerLifesBox->AddSlot();
			ImageSlot.AttachWidget(Image);
		}
	}
	else 
	{
		// Lives are visible on the screen from left to right
		// First live on the left is last live which player has

		// Therefore, ensure that every live, which is below NumLives border is visible
		for (int32 i = 0; i < NumLives; ++i)
		{
			auto Widget = PlayerLifesBox->GetChildren()->GetChildAt(i);
			Widget->SetVisibility(EVisibility::Visible);
		}

		// and the others are hidden
		for (int32 i = NumLives; i < PlayerLifesBox->NumSlots(); ++i)
		{
			auto Widget = PlayerLifesBox->GetChildren()->GetChildAt(i);
			Widget->SetVisibility(EVisibility::Hidden);
		}
	}
}

void SPacmanGameWidget::SetNumPoints(int32 NumPoints)
{
	PlayerScore->SetText("Score: " + FString::FromInt(NumPoints));
}


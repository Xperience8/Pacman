// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once


#include "Styling/SlateWidgetStyle.h"
#include "SlateWidgetStyleContainerBase.h"

#include "PacmanGameWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct PACMAN_API FPacmanGameStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FPacmanGameStyle();
	virtual ~FPacmanGameStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FPacmanGameStyle& GetDefault();

	UPROPERTY(EditAnywhere)
		FTextBlockStyle ScoreStyle;

	UPROPERTY(EditAnywhere)
		FSlateBrush LifeImage;
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UPacmanGameWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FPacmanGameStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};

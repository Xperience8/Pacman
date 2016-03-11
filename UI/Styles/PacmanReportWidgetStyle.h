// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once


#include "Styling/SlateWidgetStyle.h"
#include "SlateWidgetStyleContainerBase.h"

#include "PacmanReportWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct PACMAN_API FPacmanReportStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FPacmanReportStyle();
	virtual ~FPacmanReportStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FPacmanReportStyle& GetDefault();

	UPROPERTY(EditAnywhere)
		FTextBlockStyle Title;

	UPROPERTY(EditAnywhere)
		FTextBlockStyle Message;

	UPROPERTY(EditAnywhere)
		FButtonStyle ClickableArea;
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UPacmanReportWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FPacmanReportStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};

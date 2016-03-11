// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once


#include "Styling/SlateWidgetStyle.h"
#include "SlateWidgetStyleContainerBase.h"

#include "PacmanMenuWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct PACMAN_API FPacmanMenuStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FPacmanMenuStyle();
	virtual ~FPacmanMenuStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FPacmanMenuStyle& GetDefault();

	UPROPERTY(EditAnywhere)
		FSlateBrush BackgroundImage;

	UPROPERTY(EditAnywhere)
		FButtonStyle Button;

	UPROPERTY(EditAnywhere)
		FTextBlockStyle ButtonText;

	UPROPERTY(EditAnywhere)
		FTextBlockStyle Item;
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UPacmanMenuWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FPacmanMenuStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};

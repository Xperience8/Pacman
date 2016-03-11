// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "PacmanBehaviorComponent.generated.h"

/// Abstract class that is used as base class for behaviors
UCLASS(abstract, Blueprintable)
class PACMAN_API UPacmanBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPacmanBehaviorComponent();

	virtual void ActivateBehavior();
	virtual void OnSegmentFinished() { }
	virtual void OnPathFinished() { }

protected:
	UPROPERTY(EditAnywhere)
	float MovingSpeed;
};

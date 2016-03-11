// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "../../NavigationSystem/PacmanNavigationSystem.h"

#include "Components/ActorComponent.h"
#include "PacmanPathFollowingComponent.generated.h"


/// Component used to following predefine path
/// Ghost is always in moving, event if there is not path to follow
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PACMAN_API UPacmanPathFollowingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPacmanPathFollowingComponent();

	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	/// You must select path by GetPath() method
	void StartPathFollowing()
	{
		// If path doesn't exist or we have already been in goal location, then we don't want to follow this path 
		bFollowPath = !Path.IsAtGoal();
	}

	PathPlan &GetPath()
	{
		return Path;
	}

private:
	PathPlan Path;

	bool bFollowPath;
};

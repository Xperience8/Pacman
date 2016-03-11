// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Engine/GameInstance.h"
#include "PacmanGameInstance.generated.h"

/// PacmanGameInstance is responsible for initializing navigation system
UCLASS()
class PACMAN_API UPacmanGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	/// Create navigation system
	virtual void Init() final override;
	
	/// Navigation system can not be null
	class UPacmanNavigationSystem *GetNavigationSystem() const 
	{ 
		return NavigationSystem;
	}
private:
	UPROPERTY(Transient)
	class UPacmanNavigationSystem *NavigationSystem;
};

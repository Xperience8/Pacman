// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "PacmanPlayerController.generated.h"


UCLASS()
class PACMAN_API APacmanPlayerController : public APlayerController
{
	GENERATED_BODY()

	APacmanPlayerController() : ControllerState(EControllerState::Top), NumLives(3), NumPoints(0), RespawnDelay(5) 
	{ 
		PlayerCameraManagerClass = nullptr;
	}

public:
	/// If player has at least one live respawn him, otherwise show summary
	virtual	void BeginInactiveState() override;

	/// Doesn't spawn default camera(we don't need it)
	virtual void SpawnPlayerCameraManager() override;

	virtual void ClientReturnToMainMenu(const FString& ReturnReason) override;

	void ShowLevelSummary(bool PlayerWon);

	/// Player picked up collectible, update counter and inform game mode about it
	void AddPoints(int32 Points);

	/// Change from top controller mode to first person(or vice versa)
	void ChangeControllerMode();

	/// If controller isn't in first person mode than he is automatically in top controller mode
	bool IsInFirstPersonMode() const
	{
		return ControllerState == EControllerState::FirstPerson;
	}

private:
	/// Hide respawn screen and restart player
	void Respawn();

	enum EControllerState
	{
		FirstPerson,
		Top
	};

	EControllerState ControllerState;

	FTimerHandle RespawnTimerHandle;

	int32 NumPoints;
	int32 NumLives;
	int32 RespawnDelay;
};

// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "PacmanScalingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PACMAN_API UPacmanScalingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPacmanScalingComponent();

	/** Reference to player and Player's starting size is cached*/
	virtual void BeginPlay() override;
	
	/** Perform specific scaling operations(like growing, shrinking etc.) or just do nothing if player is in normal mode*/
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	/**
	 * It is safe to concatenate multiple scale requests
	 * If no scaling is active then player starts to grow
	 * If player has already been shrined then stop shrinking and activates growing(transform shrinking to growing)
	 * If player has already been scaled then offsets it's time
	 */
	void Scale();

private:
	/** Scale player to goal size, if scale ends, informs ghosts that player is a hunter*/
	void Grow();

	/** Only wait to end*/
	void StayBig();

	/** Scale player to starting size, if scale ens, informs ghosts that player is a prey*/
	void Shrink();

	enum class EScaleState
	{
		None,
		Grow,
		StayBig,
		Shrink
	};

	EScaleState State;

	class APacmanPlayer *Player;

	float StartingSize;

	UPROPERTY(EditAnywhere)
	float GoalSize;

	UPROPERTY(EditAnywhere)
	float TimeToScale;

	UPROPERTY(EditAnywhere)
	float TimeAsBig;

	float TimeAccumulator;
	
};

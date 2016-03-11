// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "PacmanPickup.generated.h"

/// PacmanPickup is the base abstract class for an Object that can be placed in a level 
/// and can be picked up by player 
UCLASS(abstract)
class PACMAN_API APacmanPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	/// Sets default values for this actor's properties
	APacmanPickup();

	/// Player can take pickup only once, after this call, pickup is hidden and then destroyed 
	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;
	
protected:

	/// Apply pickup effect(e.g add points etc.)
	virtual void Pickup(AActor *Player) { }

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent *Mesh;

	/// This sound is played only once, when player takes pickup
	UPROPERTY(EditDefaultsOnly)
		USoundWave *PickupSound;
};

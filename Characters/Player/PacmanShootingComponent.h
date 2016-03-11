// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "PacmanShootingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PACMAN_API UPacmanShootingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPacmanShootingComponent();

	/// If player want to shoot and shooting is enabled then projectiles are spawned every TimeBetweenShots
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	/// Shooting is enabled and we are waiting for player input to start shooting
	void EnableShooting();

	/// Player wants to shoot(he will start shooting when shooting is enabled)
	void StartShooting();

	/// Player don't want to shoot anymore
	void StopShooting()
	{
		bWantToShoot = false;
	}

private:
	/// Effect of boost run off
	void DisableShooting()
	{
		bEnabled = false;
	}

	UPROPERTY(EditAnywhere)
		TSubclassOf<class APacmanProjectile> ProjectileTemplate;

	/// Shooting is enabled only for particular period of time
	FTimerHandle DisableShootingTimerHandle;

	/// How many seconds are needed to spawn another projectile
	UPROPERTY(EditAnywhere)
		float TimeBetweenShots;

	/// How many seconds can player shoots
	UPROPERTY(EditAnywhere)
	float TimeForShooting;


	/// Check whether enough time passed between shots
	float LastShootTime;
	float TimeAccumulator;

	/// This is true when player eats boost and will be true until DisableShootingTimerHandle is called
	bool bEnabled;

	/// This is true when player click a key(for shooting) and will be true until player releases this key
	bool bWantToShoot;
};

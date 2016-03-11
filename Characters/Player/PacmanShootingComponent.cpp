// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanShootingComponent.h"

#include "../../Projectiles/PacmanProjectile.h"

// Sets default values for this component's properties
UPacmanShootingComponent::UPacmanShootingComponent() : bEnabled(false), bWantToShoot(false), TimeForShooting(10.f), TimeBetweenShots(0.5f), TimeAccumulator(0.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called every frame
void UPacmanShootingComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (bEnabled && bWantToShoot)
	{
		TimeAccumulator += DeltaTime;
		if (TimeAccumulator >= TimeBetweenShots)
		{
			TimeAccumulator -= TimeBetweenShots;
			LastShootTime = GetWorld()->GetTimeSeconds();

			APawn *Owner = Cast<APawn>(GetOwner());

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = Owner;
			SpawnParameters.Instigator = Owner;
			GetWorld()->SpawnActor<APacmanProjectile>(ProjectileTemplate, Owner->GetActorLocation() + Owner->GetActorForwardVector() * 20.f, Owner->GetActorRotation(), SpawnParameters);
		}
	}
}

void UPacmanShootingComponent::EnableShooting()
{
	// If shooting has already been enabled then we must offset it's ending time
	if (bEnabled)
	{
		float RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(DisableShootingTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(DisableShootingTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(DisableShootingTimerHandle, this, &UPacmanShootingComponent::DisableShooting, TimeForShooting + RemainingTime, false);
	}
	else
	{
		// Enable shooting and create period when shooting is enabled
		bEnabled = true;
		GetWorld()->GetTimerManager().SetTimer(DisableShootingTimerHandle, this, &UPacmanShootingComponent::DisableShooting, TimeForShooting, false);

		// No wait time to shoot
		TimeAccumulator = TimeBetweenShots;
	}
}

void UPacmanShootingComponent::StartShooting()
{
	// Check whether passed enough time, if no then offset time so player can not shoot instantly
	float ElapsedTime = GetWorld()->GetTimeSeconds() - LastShootTime;
	if (ElapsedTime < TimeBetweenShots)
	{
		TimeAccumulator = ElapsedTime;
	}
	else
	{
		TimeAccumulator = TimeBetweenShots;
	}

	bWantToShoot = true;
}
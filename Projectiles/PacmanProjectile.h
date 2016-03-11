// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "PacmanProjectile.generated.h"

/// PacmanProjectile is responsible for spawning projectile which creates explosion at hit
UCLASS()
class PACMAN_API APacmanProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	/// Sets default values for this actor's properties
	APacmanProjectile();

	/// Projectile hits ghost or wall
	/// After this call is projectile destroyed and explosion effect is created
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

private:
	UFUNCTION()
	void DestroyProjectile()
	{
		SetActorHiddenInGame(true);
		Destroy();
	}

	/// When projectile hits ghost or wall then it creates explosion which affects other actors in specific radius
	/// If player or another ghost is within this radius then he will be killed
	void ApplyRadialDamage(FVector Center, float Radius);

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent *Mesh;

	UPROPERTY(VisibleAnywhere)
		UProjectileMovementComponent *MovementComponent;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent *ExplosionEffect;

	UPROPERTY(EditDefaultsOnly)
		USoundWave *ExplosionSound;

	UPROPERTY(EditDefaultsOnly)
		float DamageImpactRadius;
};

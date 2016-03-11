// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanProjectile.h"

// Sets default values
APacmanProjectile::APacmanProjectile() : DamageImpactRadius(100.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionObjectType(ECC_PROJECTILE);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_PICKUP, ECollisionResponse::ECR_Ignore);
	
	ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>("Explosion");
	ExplosionEffect->AttachTo(Mesh);
	ExplosionEffect->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ExplosionEffect->bAutoActivate = false;
	ExplosionEffect->bAutoDestroy = true;

	// If explosion is done destroy projectile
	TScriptDelegate<> Delegate;
	Delegate.BindUFunction(this, "DestroyProjectile");
	ExplosionEffect->OnSystemFinished.Add(Delegate);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Movement");
	MovementComponent->InitialSpeed = 2000.f;
	MovementComponent->MaxSpeed = 2000.f;
	MovementComponent->UpdatedComponent = Mesh;
	MovementComponent->ProjectileGravityScale = 0.f;
}

void APacmanProjectile::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	MovementComponent->StopMovementImmediately();
		
	Mesh->SetHiddenInGame(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ExplosionEffect->Activate();
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
		
	// We don't need to shoot directly to the ghost
	ApplyRadialDamage(HitLocation, DamageImpactRadius);
}

void APacmanProjectile::ApplyRadialDamage(FVector Location, float Radius)
{
	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByObjectType(Overlaps, Location, FQuat::Identity, FCollisionObjectQueryParams::AllDynamicObjects, FCollisionShape::MakeSphere(Radius));
	for (int32 i = 0; i < Overlaps.Num(); ++i)
	{
		auto ObjectType = Overlaps[i].GetComponent()->GetCollisionObjectType();
		if (ObjectType == ECC_GHOST || ObjectType == ECollisionChannel::ECC_Pawn)
		{
			Overlaps[i].GetActor()->TakeDamage(100.f, FDamageEvent(), nullptr, GetOwner());
		}
	}
}
// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "Ghost.h"

#include "PacmanPathFollowingComponent.h"
#include "PacmanBehaviorComponent.h"

#include "BlinkyChaseBehaviorComponent.h"

#include "../Player/PacmanPlayer.h"

// Sets default values
AGhost::AGhost() : ActiveModeIndex(0)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	auto Capsule = GetCapsuleComponent();
	Capsule->SetCollisionObjectType(ECC_GHOST);
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Capsule->SetCollisionResponseToChannel(ECC_PICKUP, ECollisionResponse::ECR_Ignore);

	PathFollowingComponent = CreateDefaultSubobject<UPacmanPathFollowingComponent>("PathFollowingComponent");

	int32 NumModes = FMath::RandRange(2, 10);
	Modes.SetNum(NumModes);

	for (int32 i = 0; i < NumModes; ++i)
	{
		Modes[i].Duration = FMath::FRandRange(5.f, 20.f);
		Modes[i].GhostState = static_cast<EGhostState>(i % 2);
	}

	// Last state must be chased
	Modes.Last().GhostState = EGhostState::Chase;
	Modes.Last().Duration = 0.f;
}

void AGhost::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ensure(ScatterBehaviorTemplate && FearedBehaviorTemplate && ChaseBehaviorTemplate);

	ScatterBehavior = NewObject<UPacmanBehaviorComponent>(this, ScatterBehaviorTemplate, "ScatterBehavior");
	ScatterBehavior->RegisterComponent();

	FearedBehavior = NewObject<UPacmanBehaviorComponent>(this, FearedBehaviorTemplate, "FearedBehavior");
	FearedBehavior->RegisterComponent();

	ChaseBehavior = NewObject<UPacmanBehaviorComponent>(this, ChaseBehaviorTemplate, "ChaseBehavior");
	ChaseBehavior->RegisterComponent();
}

void AGhost::NotifyActorBeginOverlap(AActor *OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!IsPendingKill())
	{
		auto Player = static_cast<APacmanPlayer*>(OtherActor);
		
		// Timer is paused only when player is in hunt mode 
		if (Player->IsHunter())
		{
			TakeDamage(100.f, FDamageEvent(), Player->GetController(), Player);
		}
		else
		{
			Player->TakeDamage(100.f, FDamageEvent(), GetController(), this);
		}

		UGameplayStatics::PlaySound2D(this, CollisionSound);
	}
}

float AGhost::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (ShouldTakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser))
	{
		SetActorHiddenInGame(true);
		Destroy();
	}

	return 0.f;
}

bool AGhost::IsBlinky() const
{
	return ChaseBehavior->IsA<UBlinkyChaseBehaviorComponent>();
}

void AGhost::ActivateBehavior(EGhostState GhostState)
{
	switch (GhostState)
	{
	case EGhostState::Chase:
		ActiveBehavior = ChaseBehavior;
		break;
	case EGhostState::Scatter:
		ActiveBehavior = ScatterBehavior;
		break;
	case EGhostState::Feared:
		ActiveBehavior = FearedBehavior;
		break;
	}

	ActiveBehavior->ActivateBehavior();
}
void AGhost::ActivateNextMode()
{
	++ActiveModeIndex;
	ActivateBehavior(Modes[ActiveModeIndex].GhostState);

	if (ActiveModeIndex < Modes.Num() - 1)
	{
		GetWorld()->GetTimerManager().SetTimer(ChangingModeTimerHandle, this, &AGhost::ActivateNextMode, Modes[ActiveModeIndex].Duration, false);
	}
}

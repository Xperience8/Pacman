// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanScalingComponent.h"

#include "PacmanPlayer.h"
#include "../Ghost/Ghost.h"

// Sets default values for this component's properties
UPacmanScalingComponent::UPacmanScalingComponent() : StartingSize(1.f), GoalSize(1.5f), TimeToScale(0.5f), TimeAsBig(10.f), TimeAccumulator(0.f), State(EScaleState::None), Player(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPacmanScalingComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = static_cast<APacmanPlayer*>(GetOwner());
	StartingSize = Player->GetActorScale3D().X;
}


// Called every frame
void UPacmanScalingComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	TimeAccumulator += DeltaTime;

	switch (State)
	{
	case EScaleState::Grow:
		Grow();
		break;
	case EScaleState::StayBig:
		StayBig();
		break;
	case EScaleState::Shrink:
		Shrink();
		break;
	}
}

void UPacmanScalingComponent::Scale()
{
	if (State == EScaleState::None)
	{
		TimeAccumulator = 0.f;
		State = EScaleState::Grow;
	}
	else if (State == EScaleState::Shrink)
	{
		// Transform shrink to grow
		TimeAccumulator = FMath::Abs(TimeToScale - TimeAccumulator);
		State = EScaleState::Grow;
	}
	else if (State == EScaleState::StayBig)
	{
		TimeAccumulator -= TimeAsBig;
	}
}

void UPacmanScalingComponent::Grow()
{
	if (TimeAccumulator >= TimeToScale)
	{
		TimeAccumulator = 0.f;
		Player->SetActorScale3D(FVector(GoalSize, GoalSize, GoalSize));
		Player->ActivateHunterMode();
		State = EScaleState::StayBig;

		return;
	}

	float NewScale = FMath::Lerp(StartingSize, GoalSize, TimeAccumulator / TimeToScale);
	Player->SetActorScale3D(FVector(NewScale));
}
void UPacmanScalingComponent::StayBig()
{
	if (TimeAccumulator >= TimeAsBig)
	{
		TimeAccumulator = 0.f;
		State = EScaleState::Shrink;
	}
}
void UPacmanScalingComponent::Shrink()
{
	if (TimeAccumulator >= TimeToScale)
	{
		TimeAccumulator = 0.f;
		Player->SetActorScale3D(FVector(StartingSize, StartingSize, StartingSize));
		Player->ActivatePreyMode();
		State = EScaleState::None;

		return;
	}

	float NewScale = FMath::Lerp(GoalSize, StartingSize, TimeAccumulator / TimeToScale);
	Player->SetActorScale3D(FVector(NewScale));
}
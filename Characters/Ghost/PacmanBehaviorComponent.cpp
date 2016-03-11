// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanBehaviorComponent.h"

#include "Ghost.h"

// Sets default values for this component's properties
UPacmanBehaviorComponent::UPacmanBehaviorComponent() : MovingSpeed(600.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;

	// ...

}

void UPacmanBehaviorComponent::ActivateBehavior()
{
	auto Owner = static_cast<AGhost*>(GetOwner());
	auto MovementComponent = static_cast<UCharacterMovementComponent*>(Owner->GetMovementComponent());
	MovementComponent->MaxWalkSpeed = MovingSpeed;
}

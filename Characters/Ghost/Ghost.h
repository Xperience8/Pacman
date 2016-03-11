// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "../../NavigationSystem/PacmanNavigationSystem.h"

#include "GameFramework/Character.h"
#include "Ghost.generated.h"

/// Ghost tries to catch pacman and he has 3 types of behavior:
///		Feared mode -> ghost goes out of the player
///		Scatter mode -> ghost goes to home location
///		Chase mode -> ghost tries to catch player
/// Ghost switches n times between scatter and chase mode
/// if modes run out, the permanent chasing mode is activated
UCLASS()
class PACMAN_API AGhost : public ACharacter
{
	GENERATED_BODY()
private:
	enum class EGhostState : uint8
	{
		Chase,
		Scatter,
		Feared
	};

	struct FGhostMode
	{
		float Duration;

		EGhostState GhostState;
	};

public:
	AGhost();

	/// Creates behavior components from templates
	virtual void PostInitializeComponents() override;

	/// Ghost hits player(or player hits ghost)
	/// If player eats boost, ghost will be destroyed, otherwise player will be destroyed
	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;

	/// Ghost automatically dies when he takes damage
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/// Informs ghost that player is alive
	/// This method should be called when player is spawned
	void PlayerIsAlive()
	{
		// This is actually a hack for quickly reseting modes to beginning
		ActiveModeIndex = -1;
		ActivateNextMode();
	}

	/// Informs ghost that player is dead, after this call ghost goes home
	void PlayerIsDead()
	{
		ActivateBehavior(EGhostState::Scatter);
		GetWorld()->GetTimerManager().ClearTimer(ChangingModeTimerHandle);
	}

	/// Player is now hunter, therefore try to avoid him
	void PlayerIsHunter()
	{
		ActivateBehavior(EGhostState::Feared);
		GetWorld()->GetTimerManager().PauseTimer(ChangingModeTimerHandle);
	}

	/// Player is now prey, therefore try to catch him
	void PlayerIsPrey()
	{
		ActivateBehavior(Modes[ActiveModeIndex].GhostState);
		GetWorld()->GetTimerManager().UnPauseTimer(ChangingModeTimerHandle);
	}

	/// Blinky is the main ghost
	bool IsBlinky() const;

	FVector GetHomeLocation() const
	{
		return HomeLocation;
	}

	/// Return value can not be null
	class UPacmanPathFollowingComponent *GetPathFollowingComponent() const
	{
		return PathFollowingComponent;
	}

	/// Return value can not be null
	class UPacmanBehaviorComponent *GetActiveBehavior() const
	{
		return ActiveBehavior;
	}

private:
	/// Activates behavior which belongs to selected state
	void ActivateBehavior(EGhostState GhostState);

	///Next mode from the Modes array is activated and appropriate timer for choosing next mode is created
	void ActivateNextMode();

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UPacmanBehaviorComponent> ChaseBehaviorTemplate;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UPacmanBehaviorComponent> FearedBehaviorTemplate;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UPacmanBehaviorComponent> ScatterBehaviorTemplate;

	/// Modes are sequences of behavior which are activated in a row
	/// They are ignored(and paused) when player is hunter
	/// (in that case the most appropriate behavior is chosen automatically)
	/// or they are restarted, if player dies
	/// They are used to create wave effects of ghost intelligence
	TArray<FGhostMode> Modes;

	/// Which mode is currently activated, index to Modes array
	int32 ActiveModeIndex;

	/// It works together with ActivateNextMode method to gradually activate next behaviors
	FTimerHandle ChangingModeTimerHandle;

	UPROPERTY(EditAnywhere)
		USoundWave *CollisionSound;

	/// Safe location on the map
	UPROPERTY(EditAnywhere)
	FVector HomeLocation;

	UPROPERTY(VisibleAnywhere)
	class UPacmanPathFollowingComponent *PathFollowingComponent;

	UPROPERTY()
	class UPacmanBehaviorComponent *ScatterBehavior;

	UPROPERTY()
	class UPacmanBehaviorComponent *FearedBehavior;

	UPROPERTY()
	class UPacmanBehaviorComponent *ChaseBehavior;

	class UPacmanBehaviorComponent *ActiveBehavior;
};

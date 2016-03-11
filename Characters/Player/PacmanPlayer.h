// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "PacmanPlayer.generated.h"

UCLASS()
class PACMAN_API APacmanPlayer : public ACharacter
{
public:
	GENERATED_BODY()

	APacmanPlayer();
	
	/// Tell ghosts that you are alive
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/// Destroy pacman and inform ghosts that pacman is dead
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/// Change mode and inform ghosts that player is now hunter
	void ActivateHunterMode();
	
	/// Change mode and inform ghosts that player is now prey
	void ActivatePreyMode();

	bool IsHunter() const
	{
		return CharacterState == ECharacterState::Hunter;
	}

	/// Can not be null
	class UPacmanScalingComponent *GetScalingComponent() const
	{
		return ScalingComponent;
	}

	/// Can not be null
	class UPacmanShootingComponent *GetShootingComponent() const
	{
		return ShootingComponent;
	}

private:
	/// This is automatically called when bTurning is true
	/// and it rotates actor about +- 90 degrees in multiple frames
	/// This should be called only in top controller mode
	void TurnActor(float DeltaTime);

	/// Blends between two different cameras, at the end is BlendTo camera activated*/
	void BlendController(UCameraComponent *BlendFrom, UCameraComponent *BlendTo, float DeltaTime);

	/// Player can move forward only if, he is not turning
	/// In top controller mode, player can go only forward(Dir < 0.f is ignored)
	void MoveForward(float Dir);

	/// Player can move right only if he is no turning and there isn't blending between states
	/// In top controller mode, player can not move right, he can only turn right and then go forward
	void MoveRight(float Dir);

	/// Player can look up only in first person mode and only if he is not turning and there isn't blending between states
	void LookUp(float Dir);

	/// Player can turn only in first person mode and only if he is not turning and there isn't blending between states
	void Turn(float Dir);

	/// Activates blending between two controllers
	void SwitchController();

	/// Player wants to quit game
	void QuitGame();

	enum ECharacterState
	{
		Prey,		// Ghosts hunt player
		Hunter		// Player hunts ghosts
	};

	ECharacterState CharacterState;

	UPROPERTY(VisibleAnywhere)
	class UPacmanShootingComponent *ShootingComponent;

	UPROPERTY(VisibleAnywhere)
	class UPacmanScalingComponent *ScalingComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent *FpsCamera;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent *TopCamera;

	/// Only used when we are changing from first person mode to top controller mode(or vice versa)
	UCameraComponent *BlendCamera;

	class APacmanPlayerController *PlayerController;

	/// How many seconds character needs to turn(+90.f or -90.f)
	UPROPERTY(EditAnywhere)
	float TimeToTurn;

	/// How many seconds character needs to switch controller(top and first person)
	UPROPERTY(EditAnywhere)
	float BlendTime;
	
	/// Helpers for rotating actor about +- 90 degrees e.g Turning
	float TurningYawRotationStart;
	float TurningYawRotationEnd;

	float TimeAccumulator;

	bool bBlendingBetweenStates;
	bool bTurning;
};

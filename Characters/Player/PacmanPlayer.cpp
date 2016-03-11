// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanPlayer.h"

#include "PacmanPlayerController.h"

#include "PacmanScalingComponent.h"
#include "PacmanShootingComponent.h"

#include "../Ghost/Ghost.h"

APacmanPlayer::APacmanPlayer() : CharacterState(ECharacterState::Prey), BlendTime(0.5f), TimeToTurn(0.25f), TimeAccumulator(0.f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	auto Capsule = GetCapsuleComponent();
	Capsule->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECC_PICKUP, ECollisionResponse::ECR_Overlap);

	auto Mesh = GetMesh();
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FpsCamera = CreateDefaultSubobject<UCameraComponent>("FpsCamera");
	FpsCamera->AttachTo(RootComponent);
	FpsCamera->SetRelativeLocation(FVector(40.f, 0.f, 0));
	FpsCamera->bAutoActivate = false;
	FpsCamera->bUsePawnControlRotation = true;

	TopCamera = CreateDefaultSubobject<UCameraComponent>("TopCamera");
	TopCamera->AttachTo(RootComponent);
	TopCamera->SetRelativeLocation(FVector(0.f, 0.f, 1000.f));
	TopCamera->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	TopCamera->bAutoActivate = false;

	BlendCamera = CreateDefaultSubobject<UCameraComponent>("BlendCamera");
	BlendCamera->AttachTo(RootComponent);
	BlendCamera->bAutoActivate = false;

	ShootingComponent = CreateDefaultSubobject<UPacmanShootingComponent>("ShootingComponent");
	ScalingComponent = CreateDefaultSubobject<UPacmanScalingComponent>("ScalingComponent");
}

void APacmanPlayer::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = static_cast<APacmanPlayerController*>(GetController());
	if (PlayerController->IsInFirstPersonMode())
	{
		FpsCamera->Activate();
	}
	else
	{
		TopCamera->Activate();
	}

	for (TActorIterator<AGhost> GhostIt(GetWorld()); GhostIt; ++GhostIt)
	{
		GhostIt->PlayerIsAlive();
	}
}

void APacmanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bBlendingBetweenStates)
	{
		if (PlayerController->IsInFirstPersonMode())
		{
			BlendController(TopCamera, FpsCamera, DeltaTime);
		}
		else
		{
			BlendController(FpsCamera, TopCamera, DeltaTime);
		}
	}
	else if (bTurning)
	{
		TurnActor(DeltaTime);
	}
}

void APacmanPlayer::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis(TEXT("MoveForward"), this, &APacmanPlayer::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &APacmanPlayer::MoveRight);
	InputComponent->BindAxis(TEXT("LookUp"), this, &APacmanPlayer::LookUp);
	InputComponent->BindAxis(TEXT("Turn"), this, &APacmanPlayer::Turn);

	InputComponent->BindAction(TEXT("SwitchController"), EInputEvent::IE_Pressed, this, &APacmanPlayer::SwitchController);

	InputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, ShootingComponent, &UPacmanShootingComponent::StartShooting);
	InputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Released, ShootingComponent, &UPacmanShootingComponent::StopShooting);

	InputComponent->BindAction(TEXT("Quit"), EInputEvent::IE_Pressed, this, &APacmanPlayer::QuitGame);
}

float APacmanPlayer::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (ShouldTakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser))
	{
		for (TActorIterator<AGhost> GhostIt(GetWorld()); GhostIt; ++GhostIt)
		{
			GhostIt->PlayerIsDead();
		}

		SetActorHiddenInGame(true);
		Destroy();
	}

	return 0.f;
}

void APacmanPlayer::ActivateHunterMode()
{
	if (CharacterState == ECharacterState::Hunter)
	{
		return;
	}

	CharacterState = ECharacterState::Hunter;

	for (TActorIterator<AGhost> GhostIt(GetWorld()); GhostIt; ++GhostIt)
	{
		GhostIt->PlayerIsHunter();
	}
}

void APacmanPlayer::ActivatePreyMode()
{
	if (CharacterState == ECharacterState::Prey)
	{
		return;
	}

	CharacterState = ECharacterState::Prey;

	for (TActorIterator<AGhost> GhostIt(GetWorld()); GhostIt; ++GhostIt)
	{
		GhostIt->PlayerIsPrey();
	}
}

void APacmanPlayer::TurnActor(float DeltaTime)
{
	TimeAccumulator += DeltaTime;
	if (TimeAccumulator >= TimeToTurn)
	{
		TimeAccumulator = 0.f;

		FRotator NewRotation = PlayerController->GetControlRotation();
		NewRotation.Yaw = TurningYawRotationEnd;
		PlayerController->SetControlRotation(NewRotation);

		bTurning = false;
		return;
	}

	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw = FMath::Lerp(TurningYawRotationStart, TurningYawRotationEnd, TimeAccumulator / TimeToTurn);
	PlayerController->SetControlRotation(NewRotation);
}


void APacmanPlayer::BlendController(UCameraComponent *BlendFrom, UCameraComponent *BlendTo, float DeltaTime)
{
	TimeAccumulator += DeltaTime;
	if (TimeAccumulator >= BlendTime)
	{
		TimeAccumulator = 0.f;

		BlendCamera->Deactivate();
		BlendTo->Activate();

		bBlendingBetweenStates = false;
		return;
	}

	FVector newLocation = FMath::Lerp(BlendFrom->RelativeLocation, BlendTo->RelativeLocation, TimeAccumulator / BlendTime);
	BlendCamera->SetRelativeLocation(newLocation);

	FRotator newRotation = FMath::Lerp(BlendFrom->RelativeRotation, BlendTo->RelativeRotation, TimeAccumulator / BlendTime);
	BlendCamera->SetRelativeRotation(newRotation);
}

void APacmanPlayer::SwitchController()
{
	if (bBlendingBetweenStates || bTurning)
	{
		return;
	}

	if (PlayerController->IsInFirstPersonMode())
	{
		FpsCamera->Deactivate();
	}
	else
	{
		TopCamera->Deactivate();
	}

	PlayerController->ChangeControllerMode();

	BlendCamera->Activate();
	bBlendingBetweenStates = true;
}

void APacmanPlayer::QuitGame()
{
	static_cast<APlayerController*>(GetController())->ConsoleCommand("quit");
}

void APacmanPlayer::MoveForward(float Dir)
{
	if (bTurning || Dir == 0)
	{
		return;
	}

	// In top controller mode player can not go backward
	if (!PlayerController->IsInFirstPersonMode() && Dir == -1.f)
	{
		return;
	}

	GetMovementComponent()->AddInputVector(GetActorForwardVector() * Dir);
}

void APacmanPlayer::MoveRight(float Dir)
{
	if (bBlendingBetweenStates || bTurning || Dir == 0.f)
	{
		return;
	}

	if (PlayerController->IsInFirstPersonMode())
	{
		GetMovementComponent()->AddInputVector(GetActorRightVector() * Dir);
	}
	else
	{
		TurningYawRotationStart = GetController()->GetControlRotation().Yaw;
		TurningYawRotationEnd = TurningYawRotationStart + Dir * 90.f;
		bTurning = true;
	}
}

void APacmanPlayer::LookUp(float Dir)
{
	if (bBlendingBetweenStates || bTurning || Dir == 0.f)
	{
		return;
	}

	if (PlayerController->IsInFirstPersonMode())
	{
		AddControllerPitchInput(Dir);
	}
}

void APacmanPlayer::Turn(float Dir)
{
	if (bBlendingBetweenStates || bTurning || Dir == 0.f)
	{
		return;
	}

	if (PlayerController->IsInFirstPersonMode())
	{
		AddControllerYawInput(Dir);
	}
}



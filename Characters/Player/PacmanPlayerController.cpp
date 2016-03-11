// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanPlayerController.h"

#include "../../PacmanGameMode.h"

#include "UI/PacmanGameHUD.h"
#include "UI/Widgets/SPacmanGameWidget.h"

void APacmanPlayerController::BeginInactiveState()
{
	Super::BeginInactiveState();

	if (--NumLives > 0)
	{
		auto HUD = static_cast<APacmanGameHUD*>(GetHUD());
		HUD->ShowRespawnScreen(RespawnDelay);
		HUD->GetGameWidget()->SetNumLives(NumLives);
		
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &APacmanPlayerController::Respawn, static_cast<float>(RespawnDelay), false);
	
		TActorIterator<ACameraActor> CameraIt(GetWorld());
		SetViewTarget(*CameraIt);
	}
	else
	{
		ShowLevelSummary(false);
	}
}

void APacmanPlayerController::SpawnPlayerCameraManager()
{
	Super::SpawnPlayerCameraManager();

	for (TActorIterator<ACameraActor> CameraIt(GetWorld()); CameraIt; ++CameraIt)
	{
		if (CameraIt->GetActorLocation().IsZero())
		{
			CameraIt->Destroy();
			break;
		}
	}
}

void APacmanPlayerController::ClientReturnToMainMenu(const FString &ReturnReason)
{
	UGameplayStatics::OpenLevel(this, TEXT("/Game/Maps/MainMenu"));
}

void APacmanPlayerController::ShowLevelSummary(bool PlayerWon)
{
	static_cast<APacmanGameHUD*>(GetHUD())->ShowLevelSummary(PlayerWon);
	bShowMouseCursor = true;

	TActorIterator<ACameraActor> CameraIt(GetWorld());
	SetViewTarget(*CameraIt);

	// Make player invisible for ghosts
	if (PlayerWon)
	{
		GetCharacter()->DisableInput(this);
		GetCharacter()->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacter()->GetMovementComponent()->StopMovementImmediately();
	}
}

void APacmanPlayerController::AddPoints(int32 Points)
{
	NumPoints += Points;

	auto HUD = static_cast<APacmanGameHUD*>(GetHUD());
	HUD->GetGameWidget()->SetNumPoints(NumPoints);

	static_cast<APacmanGameMode*>(GetWorld()->GetAuthGameMode())->CollectibleFound();
}

void APacmanPlayerController::ChangeControllerMode()
{
	if (IsInFirstPersonMode())
	{
		ControllerState = EControllerState::Top;

		FRotator ActorRotation = GetControlRotation();
		ActorRotation.Pitch = 0.f;
		ActorRotation.Yaw = FMath::RoundToFloat(ActorRotation.Yaw / 90.f) * 90.f;

		SetControlRotation(ActorRotation);
	}
	else
	{
		ControllerState = EControllerState::FirstPerson;
	}
}

void APacmanPlayerController::Respawn()
{
	static_cast<APacmanGameHUD*>(GetHUD())->HideRespawnScreen();
	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}

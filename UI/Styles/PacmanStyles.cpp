// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanStyles.h"
#include "SlateGameResources.h"

TSharedPtr<FSlateStyleSet> FPacmanStyles::Instance = nullptr;
FName FPacmanStyles::Name = "PacmanStyles";
FString FPacmanStyles::Directory = "/Game/UI/Styles";

void FPacmanStyles::Initialize()
{
	if (Instance.IsValid())
	{
		Destroy();
	}

	Instance = FSlateGameResources::New(Name, Directory, Directory);
	FSlateStyleRegistry::RegisterSlateStyle(*Instance);
}

void FPacmanStyles::Destroy()
{
	if (Instance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*Instance);
		ensure(Instance.IsUnique());
		Instance.Reset();
	}
}
// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "UI/Styles/PacmanStyles.h"

class FPacmanGameModuleImpl : public FDefaultGameModuleImpl
{
public:
	void StartupModule() override
	{
		FPacmanStyles::Initialize();
	}
	void ShutdownModule() override
	{
		FPacmanStyles::Destroy();
	}
};


IMPLEMENT_PRIMARY_GAME_MODULE( FPacmanGameModuleImpl, Pacman, "Pacman" );

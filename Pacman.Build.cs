// Copyright 2015 Patrick Jurasek. All Rights Reserved.

using UnrealBuildTool;

public class Pacman : ModuleRules
{
	public Pacman(TargetInfo Target)
	{
        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "UMG"
            }
        );

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(
            new string[] {
                "Slate",
                "SlateCore"
            }
        );
    }
}

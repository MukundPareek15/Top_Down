// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Top_Down : ModuleRules
{
	public Top_Down(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "NetCore", "UMG" });
    }
}

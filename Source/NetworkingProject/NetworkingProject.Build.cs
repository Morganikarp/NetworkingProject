// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NetworkingProject : ModuleRules
{
	public NetworkingProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemUtils" });
	}
}

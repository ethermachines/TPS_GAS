// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TPS_GAS : ModuleRules
{
	public TPS_GAS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "GameplayAbilities", "GameplayTags", "GameplayTasks" });
	}
}

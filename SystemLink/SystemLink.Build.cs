// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SystemLink : ModuleRules
{
	public SystemLink(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara", "SlateCore", "Slate", "CommonUI", "UMG", "CommonInput"
		});

	}
}
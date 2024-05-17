// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AdvErrantLandscape : ModuleRules
{
	public AdvErrantLandscape(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"ErrantLandscape",
			}
			);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"AdvErrantLandscapeUtilities",
				"RHI",
				"RenderCore",
				"AdvErrantLandscapeShaders",
			}
			);
		
		AdvErrantLandscapeUtilities.AddErrantUtilitiesClientDefinitions(this, "Errant::Overrides");
	}
}

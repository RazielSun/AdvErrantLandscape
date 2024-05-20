using UnrealBuildTool;

public class AdvErrantLandscapeShaders : ModuleRules
{
    public AdvErrantLandscapeShaders(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Engine",
                "Projects",
                "RHI",
                "RenderCore",
                "ErrantLandscape",
            }
        );
        
        PrivateDependencyModuleNames.Add("AdvErrantLandscapeUtilities");
			
        AdvErrantLandscapeUtilities.AddErrantUtilitiesClientDefinitions(this, "Errant::Landscape");
    }
}
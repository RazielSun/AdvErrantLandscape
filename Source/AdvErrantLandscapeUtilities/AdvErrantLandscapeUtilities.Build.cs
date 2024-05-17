using UnrealBuildTool;
using System.IO;

public class AdvErrantLandscapeUtilities : ModuleRules
{
    public static bool TargetNeedsDllExportImport(ReadOnlyTargetRules Target) {
        if (Target.LinkType == TargetLinkType.Monolithic) {
            if (Target.bShouldCompileAsDLL && Target.bHasExports) {
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    }

    public static string GetDllExportImportFlag(ReadOnlyTargetRules Target) {
        return TargetNeedsDllExportImport(Target)
            ? "ERRANT_UTILITIES_WITH_DLLEXPORT_IMPORT"
            : "ERRANT_UTILITIES_NO_DLLEXPORT_IMPORT";
    }

    public static void AddErrantUtilitiesClientDefinitions(ModuleRules Module,
        string Namespace) {
        Module.PrivateDefinitions.AddRange(
            new string[] { "ERRANT_UTILITIES_NAMESPACE=" + Namespace,
                GetDllExportImportFlag(Module.Target) });
    }
    
    public AdvErrantLandscapeUtilities(ReadOnlyTargetRules Target) : base(Target)
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
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
        
        PrivateDefinitions.AddRange(new string[] {
            "ERRANT_UTILITIES_EXPORT", GetDllExportImportFlag(Target),
            "ERRANT_UTILITIES_MODULE_NAME=AdvErrantLandscapeUtilities",
            "ERRANT_UTILITIES_NAMESPACE=Errant::Overrides",
            "ERRANT_UTILITIES_MODULE_NAME_STR=\"AdvErrantLandscapeUtilities\"",
            "ERRANT_UTILITIES_PLUGIN_NAME_STR=\"AdvErrantLandscape\"",
        });
    }
}
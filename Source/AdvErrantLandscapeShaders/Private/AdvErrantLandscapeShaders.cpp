#include "CoreMinimal.h"

#include "Interfaces/IPluginManager.h"
#include "Modules/ModuleManager.h"
#include "ShaderCore.h"

#if WITH_EDITOR

namespace Errant::Landscape
{
class FAdvErrantLandscapeShadersModule final : public IModuleInterface
{
public:
	FAdvErrantLandscapeShadersModule() = default;

	virtual void StartupModule() override
	{
		const FString PluginBaseDir = IPluginManager::Get().FindPlugin(TEXT("AdvErrantLandscape"))->GetBaseDir();
		const FString LandscapeShaderDir = FPaths::Combine(PluginBaseDir, TEXT("Shaders"));
		AddShaderSourceDirectoryMapping(TEXT("/Plugin/AdvErrantLandscape"), LandscapeShaderDir);
	}
};
} // namespace Errant::Landscape

IMPLEMENT_MODULE(Errant::Landscape::FAdvErrantLandscapeShadersModule, AdvErrantLandscapeShaders);
#else  // WITH_EDITOR

IMPLEMENT_MODULE(FDefaultModuleImpl, ErrantOverridesShaders);

#endif	//WITH_EDITOR
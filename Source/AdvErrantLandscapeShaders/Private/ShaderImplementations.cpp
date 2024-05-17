#if WITH_EDITOR

#include "Shaders/SplineSDF.h"

namespace Errant::Overrides
{
	IMPLEMENT_GLOBAL_SHADER(FLandscapeSplineSDFBruteShader, "/Plugin/AdvErrantLandscape/Private/SplineSDFBrute.usf", "Main", SF_Compute);
	
}  // namespace Errant::Overrides

#endif	// WITH_EDITOR
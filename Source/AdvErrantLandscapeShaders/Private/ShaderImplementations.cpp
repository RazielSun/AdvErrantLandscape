#if WITH_EDITOR

#include "Shaders/SplineSDF.h"

namespace Errant::Landscape
{
	IMPLEMENT_GLOBAL_SHADER(FLandscapeSplineSDFBruteShader, "/Plugin/AdvErrantLandscape/Private/SplineSDFBrute.usf", "Main", SF_Compute);
	
}  // namespace Errant::Landscape

#endif	// WITH_EDITOR
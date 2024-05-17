
#pragma once

#include "CoreMinimal.h"

UE_PUSH_MACRO("ERRANT_UTILITIES_NAMESPACE")
#undef ERRANT_UTILITIES_NAMESPACE
#define ERRANT_UTILITIES_NAMESPACE Errant::Landscape
#include "ErrantLandscapeUtilities/ErrantUtilities/Public/Errant/ShaderMacros.h"
UE_POP_MACRO("ERRANT_UTILITIES_NAMESPACE")

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"

namespace Errant::Overrides
{
	struct FSplinePointInfo
	{
		FVector3f Position;
		FVector3f ArriveTangent;
		FVector3f LeaveTangent;
		int32 InterpMethod;
	};
	
	class ADVERRANTLANDSCAPESHADERS_API FLandscapeSplineSDFBruteShader final : public FGlobalShader
	{
		DECLARE_GLOBAL_SHADER(FLandscapeSplineSDFBruteShader);
		SHADER_USE_PARAMETER_STRUCT(FLandscapeSplineSDFBruteShader, FGlobalShader);
		SHOULD_COMPILE_PERMUTATION(SM5);
		MODIFY_COMPILATION_ENVIRONMENT_THREADS_XY(8, 8);
		SHADER_DRAW_EVENT_NAME("Landscape Spline SDF Brute");

		BEGIN_SHADER_PARAMETER_STRUCT(FParameters, ADVERRANTLANDSCAPESHADERS_API)
			SHADER_PARAMETER_UAV(RWTexture2D<float>, Out)
			SHADER_PARAMETER_SRV(StructuredBuffer<FSplinePointInfo>, SplinePoints)
		
			SHADER_PARAMETER(FVector4f, InActorXYInfo)
			SHADER_PARAMETER(FVector2f, InTexelToUVBias)
			SHADER_PARAMETER(FVector2f, InTexelToUVScale)
			SHADER_PARAMETER(FVector3f, InLandscapeScale)
			SHADER_PARAMETER(float, InLandscapeLocationZ)
			SHADER_PARAMETER(FVector2f, InSDFRemap)
			SHADER_PARAMETER(float, InSDFScale)
			SHADER_PARAMETER(int32, InSplinePointsCount)
			SHADER_PARAMETER(int32, InLoopSpline)
			SHADER_PARAMETER(int32, InFillInside)
			SHADER_PARAMETER(int32, Iterations)
			SHADER_PARAMETER(int32, IsHeightmap)
		
		END_SHADER_PARAMETER_STRUCT();
	};
}  // namespace Errant::Overrides
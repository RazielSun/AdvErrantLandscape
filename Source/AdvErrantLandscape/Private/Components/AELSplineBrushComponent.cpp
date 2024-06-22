// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AELSplineBrushComponent.h"

#include "Components/AELSplineComponent.h"
#include "Shaders/SplineSDF.h"

#include "ElBlendBrush.h"
#include "ElBrushPaintMask.h"

#include "Errant/RHIBufferViews.h"
#include "Errant/ShaderUtils.h"
#include "Errant/RHITexture2DViews.h"

UAELSplineBrushComponent::UAELSplineBrushComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Mask = CreateDefaultSubobject<UElBrushPaintMask>("Mask");

	VisualizationSpriteScale = FVector2d(3000);
	VisualizationColor = FLinearColor::Blue;
}

void UAELSplineBrushComponent::RenderHeightmap_RenderThread(FRHICommandListImmediate& RHICmdList,
	const FElLandscapeInfo& InLandscapeInfo, FRHITexturePool& InOutPools, const FRHITexViews& InOutCombinedResult)
{
	Render_RenderThread(RHICmdList, InLandscapeInfo, InOutPools, *InOutCombinedResult.UAV, Heightmap, true);
}

void UAELSplineBrushComponent::RenderWeightmap_RenderThread(FRHICommandListImmediate& RHICmdList,
	const FElLandscapeInfo& InLandscapeInfo, FRHITexturePool& InOutPools,
	const Errant::Landscape::FWeightmapRenderBrushInfo& InWeightmapResources, const FName& InWeightmapName)
{
	Render_RenderThread(RHICmdList, InLandscapeInfo, InOutPools, *InWeightmapResources.CurrentWeightmapUAV,
		Weightmaps.FindChecked(InWeightmapName), false);
}

void UAELSplineBrushComponent::SetSplineComponent(const TObjectPtr<UAELSplineComponent>& InSplineComponent)
{
	if (InSplineComponent)
	{
		CurrentSplineComponent = InSplineComponent;
		if (!SplineEditedDelegate.IsValid())
		{
			SplineEditedDelegate = CurrentSplineComponent->OnSplineEdited.AddUObject(this, &ThisClass::OnSplineEdited);
		}
	}
}

bool UAELSplineBrushComponent::AllowsPartialUpdate() const
{
	return true;
}

UElBrushPaintMask* UAELSplineBrushComponent::GetBrushPaintMask() const
{
	return Mask;
}

void UAELSplineBrushComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	using namespace Errant::Landscape;

	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetPropertyName();

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UAELSplineBrushComponent, Weightmaps))
	{
		ValidateWeightmapsOnTheNextTick();
	}
}

void UAELSplineBrushComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);

	// GetOrCreateRT();
}

void UAELSplineBrushComponent::Render_RenderThread(
	FRHICommandListImmediate& RHICmdList,
	const FElLandscapeInfo& InLandscapeInfo,
	IElBrushInterface::FRHITexturePool& InOutPools,
	FRHIUnorderedAccessView& InOutCombinedResult,
	const FBELandscapeSplineBrushParams& BrushParams,
	const bool InIsHeightmap) const
{
	using namespace Errant;
	using namespace Landscape;

	if (!CurrentSplineComponent.IsValid())
	{
		return;
	}

	auto [LandscapeRTCopyInfo, RTSize, FullRTSize, FullRTOffset] = GetRenderInfo(InLandscapeInfo);

	if (!LandscapeRTCopyInfo)
	{
		// We're outside landscape
		return;
	}

	const FTexture2DViewsRHIRef OutViews =
		RHICreateTexture2DViews(RTSize.X, RTSize.Y, PF_R32_FLOAT, 1, 1, TexCreate_UAV | TexCreate_RenderTargetable, TEXT("OutTexture"));

	const FIntPoint CombinedResultOffset = GetLandscapeRTOffset(*LandscapeRTCopyInfo);

	{
		/*
		 * SDF SPLINE GEN
		 */
		RHICmdList.Transition({FRHITransitionInfo(OutViews->Texture, ERHIAccess::Unknown, ERHIAccess::UAVMask)});

		TArray<FSplinePointInfo> SplinePointsInfos;
		const int32 TotalPoints = CurrentSplineComponent->GetNumberOfSplinePoints();
		SplinePointsInfos.Reserve(TotalPoints);
		for (int32 Index = 0; Index < TotalPoints; Index++)
		{
			ESplinePointType::Type PointType = CurrentSplineComponent->GetSplinePointType(Index);
			const auto PointTransform = CurrentSplineComponent->GetTransformAtSplinePoint(Index, ESplineCoordinateSpace::World);
			const auto ArriveTangent = CurrentSplineComponent->GetArriveTangentAtSplinePoint(Index, ESplineCoordinateSpace::World);
			const auto LeaveTangent = CurrentSplineComponent->GetLeaveTangentAtSplinePoint(Index, ESplineCoordinateSpace::World);
			SplinePointsInfos.Add({FVector3f(PointTransform.GetLocation()), FVector3f(ArriveTangent), FVector3f(LeaveTangent), static_cast<int32>(PointType)});
		}

		constexpr uint32 PointInfoSize = sizeof(FSplinePointInfo);
		const FBufferViewsRHIRef SplinePointsViews = RHICreateBufferViews(
			PointInfoSize,
			PointInfoSize * CurrentSplineComponent->GetNumberOfSplinePoints(),
			EBufferUsageFlags::ShaderResource | EBufferUsageFlags::StructuredBuffer,
			SplinePointsInfos.GetData(),
			TEXT("Spline Points Info"));

		FLandscapeSplineSDFBruteShader::FParameters SDFParams;
		SDFParams.Out = OutViews->UAV;;
		SDFParams.SplinePoints = SplinePointsViews->SRV;

		SDFParams.InTexelToUVBias = FVector2f(FullRTOffset.X, FullRTOffset.Y);
		SDFParams.InTexelToUVScale = FVector2f(1.F) / FVector2f(FullRTSize.X - 1, FullRTSize.Y - 1);
		const FVector BrushScale = GetComponentScale() * 100.0f;
		SDFParams.InActorXYInfo = FVector4f(GetComponentLocation().X, GetComponentLocation().Y, BrushScale.X, BrushScale.Y);
		SDFParams.InSDFScale = 1.0f / BrushParams.SDFSlopeSize;
		SDFParams.InSplinePointsCount = TotalPoints;
		SDFParams.InLoopSpline = CurrentSplineComponent->IsClosedLoop();
		SDFParams.InFillInside = BrushParams.bFillInside ? 1 : 0;
		SDFParams.Iterations = BrushParams.CurveIterations;
		SDFParams.InSDFRemap = FVector2f(BrushParams.SDFRemap.X, BrushParams.SDFRemap.Y);
		SDFParams.InLandscapeLocationZ = GetOwningLandscape() ? GetOwningLandscape()->GetActorLocation().Z : 0;;
		SDFParams.InLandscapeScale = GetOwningLandscape() ? FVector3f(GetOwningLandscape()->GetActorScale()) : FVector3f(100);
		SDFParams.IsHeightmap = (InIsHeightmap) ? 1 : 0;

		ShaderDispatch<FLandscapeSplineSDFBruteShader>(RHICmdList, SDFParams, RTSize.X, RTSize.Y);

		RHICmdList.Transition({FRHITransitionInfo(OutViews->Texture, ERHIAccess::UAVMask, ERHIAccess::SRVMask)});
	}

	RHICmdList.Transition({FRHITransitionInfo(&InOutCombinedResult, ERHIAccess::UAVMask, ERHIAccess::UAVMask)});

	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	// ReSharper disable once CppDeprecatedEntity
	BlendBrush(
		RHICmdList,
		BrushParams.BlendSettings,
		InIsHeightmap,
		GetComponentLocation().Z,
		CombinedResultOffset,
		*OutViews->Texture,
		InOutCombinedResult,
		FullRTSize,
		FullRTOffset,
		Mask->GetOutputRHI());
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void UAELSplineBrushComponent::OnSplineEdited()
{
	RequestUpdateLandscape();
}

TSet<FName> UAELSplineBrushComponent::GetWeightmaps() const
{
	TSet<FName> WeightmapsSet;
	Weightmaps.GetKeys(WeightmapsSet);
	return WeightmapsSet;
}
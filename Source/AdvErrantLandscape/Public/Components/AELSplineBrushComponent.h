
#pragma once

#include "CoreMinimal.h"
#include "ElBlendBrush.h"
#include "ElBrushComponent.h"

#include "AELSplineBrushComponent.generated.h"

class UAELSplineBrushComponent;
class UAELSplineComponent;
class UElBrushPaintMask;

USTRUCT(BlueprintType)
struct FBELandscapeSplineBrushParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Landscape Brush")
	float SDFSlopeSize = 2048.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Landscape Brush")
	FVector2D SDFRemap = FVector2D(0.0, 0.5);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Landscape Brush")
	int32 CurveIterations = 100;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Landscape Brush")
	bool bFillInside = true;

	// UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Landscape Brush")
	// FElBlendBrushSettings2 BlendSettingsV2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Landscape Brush")
	FElBlendBrushSettings_DEPRECATED BlendSettings;
};

UCLASS()
class UAELSplineBrushComponent : public UElBrushComponent
{
	GENERATED_BODY()

	friend class AAELBaseSplineBrushActor;

public:
	UAELSplineBrushComponent();

	virtual void RenderHeightmap_RenderThread(
			FRHICommandListImmediate& RHICmdList,
			const FElLandscapeInfo& InLandscapeInfo,
			FRHITexturePool& InOutPools,
			const FRHITexViews& InOutCombinedResult) override;
	virtual void RenderWeightmap_RenderThread(
		FRHICommandListImmediate& RHICmdList,
		const FElLandscapeInfo& InLandscapeInfo,
		FRHITexturePool& InOutPools,
		const Errant::Landscape::FWeightmapRenderBrushInfo& InWeightmapResources,
		const FName& InWeightmapName) override;

	void SetSplineComponent(const TObjectPtr<UAELSplineComponent>& InSplineComponent);
	
	virtual bool AllowsPartialUpdate() const override;
	virtual UElBrushPaintMask* GetBrushPaintMask() const override;
	virtual TSet<FName> GetWeightmaps() const override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landscape Brush")
	FBELandscapeSplineBrushParams Heightmap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landscape Brush")
	TMap<FName, FBELandscapeSplineBrushParams> Weightmaps;

protected:
	
	void Render_RenderThread(
		FRHICommandListImmediate& RHICmdList,
		const FElLandscapeInfo& InLandscapeInfo,
		IElBrushInterface::FRHITexturePool& InOutPools,
		FRHIUnorderedAccessView& InOutCombinedResult,
		const FBELandscapeSplineBrushParams& BrushParams,
		const bool InIsHeightmap) const;

	UFUNCTION()
	void OnSplineEdited();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Landscape Brush", Instanced)
	TObjectPtr<UElBrushPaintMask> Mask = nullptr;

	UPROPERTY(Transient)
	TWeakObjectPtr<UAELSplineComponent> CurrentSplineComponent = nullptr;

	FDelegateHandle SplineEditedDelegate;
};

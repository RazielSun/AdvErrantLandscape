#pragma once

#include "CoreMinimal.h"
#include "ElBrushActor.h"

#include "AELBaseSplineBrushActor.generated.h"

UCLASS(Abstract)
class ADVERRANTLANDSCAPE_API AAELBaseSplineBrushActor : public AElBrushActor
{
	GENERATED_BODY()

public:
	AAELBaseSplineBrushActor();

	virtual void RegisterAllComponents() override;
	virtual void PostLoad() override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<class UAELSplineComponent> SplineComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Landscape Brush")
	TObjectPtr<class UAELSplineBrushComponent> SplineBrushComponent = nullptr;
};
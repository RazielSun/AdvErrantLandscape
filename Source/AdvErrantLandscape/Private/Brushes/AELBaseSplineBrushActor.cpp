#include "Brushes/AELBaseSplineBrushActor.h"

#include "Components/AELSplineComponent.h"
#include "Components/AELSplineBrushComponent.h"

AAELBaseSplineBrushActor::AAELBaseSplineBrushActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineBrushComponent = CreateDefaultSubobject<UAELSplineBrushComponent>(TEXT("Spline Brush Component"));
	SplineBrushComponent->Mobility = EComponentMobility::Static;
	SetRootComponent(SplineBrushComponent);
	SplineBrushComponent->SetWorldScale3D({128, 128, 1});

	SplineBrushComponent->BrushPriority = UINT_MAX;

	SplineComponent = CreateDefaultSubobject<UAELSplineComponent>(TEXT("Spline Component"));
	SplineComponent->Mobility = EComponentMobility::Static;
	SplineComponent->SetupAttachment(RootComponent);
	SplineComponent->SetUsingAbsoluteScale(true);
}

void AAELBaseSplineBrushActor::RegisterAllComponents()
{
	Super::RegisterAllComponents();

	if (SplineBrushComponent && SplineComponent)
	{
		SplineBrushComponent->SetSplineComponent(SplineComponent);
	}
}

void AAELBaseSplineBrushActor::PostLoad()
{
	Super::PostLoad();

	if (SplineBrushComponent && SplineComponent)
	{
		SplineBrushComponent->SetSplineComponent(SplineComponent);
	}
}
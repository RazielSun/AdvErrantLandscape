// Fill out your copyright notice in the Description page of Project Settings.


#include "Brushes/AELRoadBrushActor.h"

#include "Components/AELSplineComponent.h"
#include "Components/AELSplineBrushComponent.h"

AAELRoadBrushActor::AAELRoadBrushActor()
{
	SplineComponent->SetClosedLoop(false, false);

	TArray<FVector> Points;
	const float Size = 30.0f * 128.f;
	Points.Add(FVector(Size, Size, 0));
	Points.Add(FVector(0, Size, 0));
	Points.Add(FVector(-Size, 0, 0));
	SplineComponent->SetSplinePoints(Points, ESplineCoordinateSpace::Local);
}
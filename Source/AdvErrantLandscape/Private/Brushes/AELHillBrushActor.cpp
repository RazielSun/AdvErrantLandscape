// Fill out your copyright notice in the Description page of Project Settings.


#include "Brushes/AELHillBrushActor.h"

#include "Components/AELSplineComponent.h"

AAELHillBrushActor::AAELHillBrushActor()
{
	SplineComponent->SetClosedLoop(true, false);

	TArray<FVector> Points;
	const float Size = 25.0f * 128.f;
	Points.Add(FVector(Size, 0, 0));
	Points.Add(FVector(0, Size, 0));
	Points.Add(FVector(-Size, 0, 0));
	Points.Add(FVector(0, -Size, 0));
	SplineComponent->SetSplinePoints(Points, ESplineCoordinateSpace::Local);
}
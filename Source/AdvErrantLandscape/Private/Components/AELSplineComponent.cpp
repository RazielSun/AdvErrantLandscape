// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AELSplineComponent.h"

UAELSplineComponent::UAELSplineComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UAELSplineComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (OnSplineEdited.IsBound())
	{
		OnSplineEdited.Broadcast();
	}
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Misc/CoreDelegates.h"

#include "AELSplineComponent.generated.h"


UCLASS()
class UAELSplineComponent : public USplineComponent
{
	GENERATED_BODY()

public:
	UAELSplineComponent();

	// UObject interface
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	// virtual void PostLoad() override;
	// End of UObject interface

	DECLARE_MULTICAST_DELEGATE(FOnSplineEdited);

	FOnSplineEdited OnSplineEdited;
};
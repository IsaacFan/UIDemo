// Copyright CL Chen - All Rights Reserved.

#pragma once

#include <UObject/Interface.h>

#include "DMUIAnalogCursorInteractableWidgetInterface.generated.h"

class UWidget;

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class UDMUIAnalogCursorInteractableWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class IDMUIAnalogCursorInteractableWidgetInterface
{
	GENERATED_BODY()

protected:
	virtual float GetAnalogCursorIncreaseFriction() const;
	void ApplyCursorFriction(const UWidget& UIWidget, const float IncreaseFriction);
	void ResetCursorFriction(const UWidget& UIWidget);
	void ApplyCursorScale(const UWidget& UIWidget, const float Scale);
	void ResetCursorScale(const UWidget& UIWidget);

	bool bIsAnalogCursorFrictionApplied = false;
	bool bIsCursorScaleApplied = false;
};
// Copyright CL Chen - All Rights Reserved.

#include "UI/Core/CommonUI/DMUIAnalogCursorInteractableWidgetInterface.h"

#include <Components/Widget.h>

#include "UI/Helpers/WDUIAnalogCursorHelpers.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DMUIAnalogCursorInteractableWidgetInterface)

float IDMUIAnalogCursorInteractableWidgetInterface::GetAnalogCursorIncreaseFriction() const
{
	return 0.0f;
}

void IDMUIAnalogCursorInteractableWidgetInterface::ApplyCursorFriction(const UWidget& UIWidget, const float IncreaseFriction)
{
	if (FMath::IsNearlyEqual(IncreaseFriction, 0.f) == true)
	{
		return;
	}

	UWDUIAnalogCursorHelpers::AddCursorFriction(&UIWidget, IncreaseFriction);
	bIsAnalogCursorFrictionApplied = true;
}

void IDMUIAnalogCursorInteractableWidgetInterface::ResetCursorFriction(const UWidget& UIWidget)
{
	if (bIsAnalogCursorFrictionApplied == false)
	{
		return;
	}

	UWDUIAnalogCursorHelpers::ResetCursorFriction(&UIWidget);
	bIsAnalogCursorFrictionApplied = false;
}

void IDMUIAnalogCursorInteractableWidgetInterface::ApplyCursorScale(const UWidget& UIWidget, const float Scale)
{
	if (FMath::IsNearlyEqual(Scale, 0.f) == true)
	{
		return;
	}

	UWDUIAnalogCursorHelpers::SetCursorScale(&UIWidget, Scale);
	bIsCursorScaleApplied = true;
}

void IDMUIAnalogCursorInteractableWidgetInterface::ResetCursorScale(const UWidget& UIWidget)
{
	if (bIsCursorScaleApplied == false)
	{
		return;
	}

	UWDUIAnalogCursorHelpers::ResetCursorScale(&UIWidget);
	bIsCursorScaleApplied = false;
}

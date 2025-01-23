// Copyright CL Chen - All Rights Reserved.

#include "UI/Core/DMUISafeZoneContainer.h"

#include <Components/CanvasPanelSlot.h>
#include <Engine/World.h>
#if !UE_BUILD_SHIPPING
#include <Blueprint/WidgetTree.h>
#include <Components/Image.h>
#endif //!UE_BUILD_SHIPPING

#include "Game/DMGameUserSettings.h"

void UDMUISafeZoneContainer::UpdateSafeZoneScope()
{
	UCanvasPanelSlot* const SafeZoneCanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot);
	if (!ensure(SafeZoneCanvasPanelSlot))
	{
		return;
	}
	
	UDMGameUserSettings* const Settings = Cast<UDMGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	if (!IsValid(Settings))
	{
		return;
	}

	const float HalfValue = (1.0f - Settings->GetSafeZonePercentage()) * 0.5f;
	const float OneMinusHalf = 1.0f - HalfValue;
	FAnchors SafeZoneAnchors;
	SafeZoneAnchors.Minimum.Set(HalfValue, HalfValue);
	SafeZoneAnchors.Maximum.Set(OneMinusHalf, OneMinusHalf);
	SafeZoneCanvasPanelSlot->SetSize(FVector2D(0.0f, 0.0f));
	SafeZoneCanvasPanelSlot->SetAnchors(SafeZoneAnchors);
}

void UDMUISafeZoneContainer::RegisterEvents()
{
	UWorld* const World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	/*
	UpdateSafeZonePercentageHandle = Event::FWorldEventHelpers::RegisterUObject(
		*World, *this, &UDMUISafeZoneContainer::OnUpdateSafeZonePercentageCallback);

#if !UE_BUILD_SHIPPING
	SwitchSafeZoneVisibilityHandle = Event::FWorldEventHelpers::RegisterUObject(
		*World, *this, &UDMUISafeZoneContainer::SwitchSafeZoneVisibilityCallback);
#endif //!UE_BUILD_SHIPPING
	*/
}

void UDMUISafeZoneContainer::UnregisterEvents()
{
	/*
	UpdateSafeZonePercentageHandle.Reset();
#if !UE_BUILD_SHIPPING
	SwitchSafeZoneVisibilityHandle.Reset();
#endif //!UE_BUILD_SHIPPING
	*/
}

void UDMUISafeZoneContainer::OnUpdateSafeZonePercentageCallback(
	const DMUISceneEvents::FOnSafeZonePercentageChangedEvent& Event)
{
	UpdateSafeZoneScope();
}

#if !UE_BUILD_SHIPPING
void UDMUISafeZoneContainer::SwitchSafeZoneVisibilityCallback(
	const DMUISceneEvents::FSwitchSafeZoneVisibilityEvent& Event)
{
	if (!IsValid(SafeZoneDebugBlockImage))
	{
		UWidgetTree* const WidgetTree = Cast<UWidgetTree>(GetOuter());
		if (IsValid(WidgetTree))
		{
			SafeZoneDebugBlockImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
			AddChild(SafeZoneDebugBlockImage);

			UCanvasPanelSlot* const ImageCanvasPanelSlot = Cast<UCanvasPanelSlot>(SafeZoneDebugBlockImage->Slot);
			if (IsValid(ImageCanvasPanelSlot))
			{
				FAnchors FullScreenAnchors;
				FullScreenAnchors.Minimum.Set(0.f, 0.f);
				FullScreenAnchors.Maximum.Set(1.f, 1.f);
				ImageCanvasPanelSlot->SetSize(FVector2D(0.0f, 0.0f));
				ImageCanvasPanelSlot->SetAnchors(FullScreenAnchors);
			}

			FLinearColor NewColor = SafeZoneDebugBlockImage->GetColorAndOpacity();
			NewColor.A = 0.2f;
			SafeZoneDebugBlockImage->SetColorAndOpacity(NewColor);
		}
	}

	SafeZoneDebugBlockImage->SetVisibility(Event.Visibility == true ?
		ESlateVisibility::HitTestInvisible : 
		ESlateVisibility::Collapsed);
}
#endif //!UE_BUILD_SHIPPING


FAutoConsoleCommandWithWorldAndArgs DMUpdateUISafeZonePercentageCmd(
	TEXT("UI.SafeZone.UpdatePercentage"),
	TEXT("Give a new percentage and update the safe zone scope."),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* const World)
	{
		UDMGameUserSettings* const Settings = Cast<UDMGameUserSettings>(UGameUserSettings::GetGameUserSettings());
		if (!IsValid(Settings))
		{
			return;
		}

		if ((Args.Num() <= 0))
		{
			return;
		}

		float NewPercentage = 0;	
		LexTryParseString<float>(NewPercentage, *Args[0]);
		Settings->SetSafeZonePercentage(World, NewPercentage);
	})
);

#if !UE_BUILD_SHIPPING
FAutoConsoleCommandWithWorldAndArgs DMSwitchSafeZoneVisibilityCmd(
	TEXT("UI.SafeZone.SwitchScopeVisibility"),
	TEXT("Switch on/off safe zone debug scope visibility."),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* const World)
	{
		if (!IsValid(World))
		{
			return;
		}

		if (Args.Num() <= 0)
		{
			return;
		}
		/*
		DMUISceneEvents::FSwitchSafeZoneVisibilityEvent Event;
		Event.Visibility = FCString::Atoi(*Args[0]) <= 0 ? false : true;
		Event::FWorldEventHelpers::Report(*World, Event);
		*/
	})
);
#endif //UE_BUILD_SHIPPING
// Copyright CL Chen - All Rights Reserved.

#include "UI/Core/CommonUI/DMCommonUIAnalogCursor.h"

#include <CommonInputSubsystem.h>
#include <CommonInputTypeEnum.h>
#include <Blueprint/UserWidget.h>
#include <Blueprint/WidgetTree.h>
#include <Components/Image.h>
#include <Components/Widget.h>
#include <Engine/Console.h>
#include <Engine/GameViewportClient.h>
#include <Engine/LocalPlayer.h>
#include <Engine/World.h>
#include <Framework/Application/SlateApplication.h>
#include <Framework/Application/SlateUser.h>
#include <GameFramework/PlayerController.h>
#include <Slate/SObjectWidget.h>

#include "UI/Core/CommonUI/DMCommonUISettings.h"
#include "UI/Core/SceneManagement/DMUISceneManager.h"
#include "UI/Helpers/DMUIFunctionLibrary.h"
#include "UI/Core/CommonUI/DMCommonUIActionRouter.h"

FDMCommonAnalogCursor::FDMCommonAnalogCursor(const UCommonUIActionRouterBase& InActionRouter)
	: FCommonAnalogCursor(InActionRouter)
{
	CachedAnalogCursorSettings = UCommonUIInputSettings::Get().GetAnalogCursorSettings();
}

void FDMCommonAnalogCursor::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{
	UpdateLastCursorPosition();

	UDMCommonUISettings& Settings = UDMCommonUISettings::Get();
	if (!IsVirtualCursorOverriden())
	{
		FCommonAnalogCursor::Tick(DeltaTime, SlateApp, Cursor);
	}
}

bool FDMCommonAnalogCursor::ShouldVirtualAcceptSimulateMouseButton(const FKeyEvent& InKeyEvent, EInputEvent InputEvent) const
{
	return !IsVirtualCursorOverriden();
}

bool FDMCommonAnalogCursor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	if (IsRelevantInput(InKeyEvent))
	{
		const ULocalPlayer& LocalPlayer = *ActionRouter.GetLocalPlayerChecked();
		if (IsValid(LocalPlayer.ViewportClient) && 
			IsValid(LocalPlayer.ViewportClient->ViewportConsole) && 
			LocalPlayer.ViewportClient->ViewportConsole->ConsoleActive())
		{
			// Let everything through when the console is open.
			return false;
		}

		HandleAnalogCursorStateByPressedKey(InKeyEvent.GetKey());

		// We support binding actions to the virtual accept key, so it's a special flower that gets processed right now
		const bool bIsVirtualAccept = InKeyEvent.GetKey() == EKeys::Virtual_Accept;
		const EInputEvent InputEventType = InKeyEvent.IsRepeat() ? IE_Repeat : IE_Pressed;
		if (bIsVirtualAccept && ActionRouter.ProcessInput(InKeyEvent.GetKey(), InputEventType) == ERouteUIInputResult::Handled)
		{
			return true;
		}
		else if (!bIsVirtualAccept || ShouldVirtualAcceptSimulateMouseButton(InKeyEvent, IE_Pressed))
		{
			// There is no awareness on a mouse event of whether it's real or not, so mark that here.
			UCommonInputSubsystem& InputSubsytem = ActionRouter.GetInputSubsystem();
			InputSubsytem.SetIsGamepadSimulatedClick(bIsVirtualAccept);
			bool bReturnValue = FAnalogCursor::HandleKeyDownEvent(SlateApp, InKeyEvent);
			InputSubsytem.SetIsGamepadSimulatedClick(false);

			return bReturnValue;
		}
	}
	return false;
}

void FDMCommonAnalogCursor::UpdateForceHiddenSwitchOfAnalogCursor(const bool bIsForceHidden)
{
	bIsAnalogForcedHidden = bIsForceHidden;

	if (bIsForceHidden == true)
	{
		bEnableAnalogMovementAfterForceHidden = bIsAnalogMovementEnabled;
		if (bIsAnalogMovementEnabled == true)
		{
			bIsAnalogMovementEnabled = false;
		}
	}
	else
	{
		if (bEnableAnalogMovementAfterForceHidden == true)
		{
			bIsAnalogMovementEnabled = true;
			bEnableAnalogMovementAfterForceHidden = false;
		}
	}
}

float FDMCommonAnalogCursor::GetStickySlowdown() const
{
	return StickySlowdown;
}

void FDMCommonAnalogCursor::ResetStickySlowdown()
{
	SetStickySlowdown(CachedAnalogCursorSettings.HoverSlowdownFactor);
}

void FDMCommonAnalogCursor::SetCursorScale(const float Scale, const bool bIsOnlyForAnalogCursorMode)
{
	if (CurrentCursorScale == Scale)
	{
		return;
	}

	CurrentCursorScale = Scale;

	if (bIsOnlyForAnalogCursorMode == true && bIsAnalogMovementEnabled == false)
	{
		return;
	}

	ApplyCursorScale();
}

void FDMCommonAnalogCursor::ResetCursorScale()
{
	SetCursorScale(1.f, false);
}

void FDMCommonAnalogCursor::RefreshCursorSettings()
{
	if (HasSetAnalogCursorSetting == true)
	{
		return;
	}

	FCommonAnalogCursor::RefreshCursorSettings();
	HasSetAnalogCursorSetting = true;
}

void FDMCommonAnalogCursor::HandleInputMethodChanged(ECommonInputType NewInputMethod)
{
	if (ActiveInputMethod == NewInputMethod)
	{
		FCommonAnalogCursor::HandleInputMethodChanged(NewInputMethod);
		return;
	}

	if (NewInputMethod == ECommonInputType::MouseAndKeyboard && bIsAnalogMovementEnabled == true)
	{
		DisableAnalogCursor();
	}

	FCommonAnalogCursor::HandleInputMethodChanged(NewInputMethod);
}

void FDMCommonAnalogCursor::HandleAnalogCursorStateByPressedKey(const FKey& PressedKey)
{
	if (bIsAnalogForcedHidden == true)
	{
		return;
	}

	if (bIsAnalogMovementEnabled == false)
	{
		if (PressedKey == EKeys::Gamepad_LeftThumbstick)
		{
			EnableAnalogCursor();
		}
		else
		{
			if (bIsAnalogNavigationEnabled == false)
			{
				if (PressedKey == EKeys::Gamepad_LeftStick_Up ||
					PressedKey == EKeys::Gamepad_LeftStick_Down ||
					PressedKey == EKeys::Gamepad_LeftStick_Right ||
					PressedKey == EKeys::Gamepad_LeftStick_Left)
				{
					EnableAnalogCursor();
				}
			}
		}
	}
	else
	{
		if (PressedKey == EKeys::Gamepad_LeftThumbstick)
		{
			EnableAnalogStickNavigation();
		}
		else if (PressedKey == EKeys::Gamepad_DPad_Up ||
				 PressedKey == EKeys::Gamepad_DPad_Down ||
				 PressedKey == EKeys::Gamepad_DPad_Right ||
				 PressedKey == EKeys::Gamepad_DPad_Left)
		{
			DisableAnalogCursor();
		}
	}
}

void FDMCommonAnalogCursor::EnableAnalogCursor()
{
	bIsAnalogMovementEnabled = true;
	bIsAnalogNavigationEnabled = false;
	ApplyCursorScale();
	
	FSlateApplication& SlateApp = FSlateApplication::Get();
	const TSharedRef<FSlateUser> SlateUser = SlateApp.GetUser(GetOwnerUserIndex()).ToSharedRef();
	SlateUser->SetCursorPosition(LastCursorPosition);
}

void FDMCommonAnalogCursor::DisableAnalogCursor()
{
	bIsAnalogMovementEnabled = false;
	ResetCursorScale();

	FSlateApplication& SlateApp = FSlateApplication::Get();
	const TSharedRef<FSlateUser> SlateUser = SlateApp.GetUser(GetOwnerUserIndex()).ToSharedRef();
	SlateUser->SetCursorPosition(LastCursorPosition);
}

void FDMCommonAnalogCursor::EnableAnalogStickNavigation()
{
	bIsAnalogMovementEnabled = false;
	bIsAnalogNavigationEnabled = true;
}

bool FDMCommonAnalogCursor::IsVirtualCursorOverriden() const
{
	const UDMCommonUISettings& Settings = UDMCommonUISettings::Get();
	if (!Settings.IsVirtualCursorOverrideEnabled())
	{
		return false;
	}

	const ULocalPlayer* const LocalPlayer = ActionRouter.GetLocalPlayer();
	if (!IsValid(LocalPlayer))
	{
		return false;
	}

	const APlayerController* const PlayerController = LocalPlayer->GetPlayerController(LocalPlayer->GetWorld());
	if (UDMUISceneManager* UISceneManager = UDMUIFunctionLibrary::GetSceneManager(PlayerController))
	{
		return Settings.IsSceneTagInVirtualCursorOverrides(UISceneManager->GetCurrentSceneTag());
	}

	// Default behavior from base common analog cursor.
	return false;
}

UWidget* FDMCommonAnalogCursor::GetCursorWidget()
{
	if (CachedCursorWidget.IsValid())
	{
		return CachedCursorWidget.Get();
	}

	const UWorld* const World = ActionRouter.GetWorld();
	const UGameViewportClient* const GameViewportClient = IsValid(World) ? World->GetGameViewport() : nullptr;
	if (IsValid(GameViewportClient) == false)
	{
		return nullptr;
	}

	TSharedPtr<SWidget> CurrentCursorWidget = GameViewportClient->GetSoftwareCursorWidget(EMouseCursor::Type::Default);
	TSharedPtr<SObjectWidget> ObjectWidget = StaticCastSharedPtr<SObjectWidget>(CurrentCursorWidget);
	UUserWidget* const CursorUserWidget = ObjectWidget.IsValid() ? ObjectWidget->GetWidgetObject() : nullptr;
	const UWidgetTree* const CursorWidgetTree = IsValid(CursorUserWidget) ? CursorUserWidget->WidgetTree : nullptr;
	if (IsValid(CursorWidgetTree) == false)
	{
		return nullptr;
	}

	CursorWidgetTree->ForEachWidget([&](UWidget* Widget) 
	{
		if (IsValid(Widget) && Widget->IsA<UImage>())
		{
			CachedCursorWidget = Widget;
			return;
		}
	});

	return CachedCursorWidget.Get();
}

void FDMCommonAnalogCursor::ApplyCursorScale()
{
	UWidget* const CursorWidget = GetCursorWidget();
	if (IsValid(CursorWidget))
	{
		CursorWidget->SetRenderScale(FVector2D(CurrentCursorScale, CurrentCursorScale));
	}
}

void FDMCommonAnalogCursor::UpdateLastCursorPosition()
{
	FSlateApplication& SlateApp = FSlateApplication::Get();
	const TSharedPtr<FSlateUser> SlateUserPtr = SlateApp.GetUser(GetOwnerUserIndex());
	if (!SlateUserPtr.IsValid())
	{
		return;
	}
	const TSharedRef<FSlateUser> SlateUser = SlateUserPtr.ToSharedRef();
	const FVector2D LocalLastCursorPosition = SlateUser->GetCursorPosition();

	if (LocalLastCursorPosition.X > FLT_MIN && LocalLastCursorPosition.Y > FLT_MIN)
	{
		LastCursorPosition = LocalLastCursorPosition;
	}

	if (bIsAnalogMovementEnabled)
	{
		// we need to keep the platform cursor in-sync. Currently slate user is using slate cursor,
		// but when we switch back we need to continue from the same position and there is not a clean place to listen for that switch.
		if (TSharedPtr<ICursor> PlatformCursor = SlateApp.GetPlatformCursor())
		{
			PlatformCursor->SetPosition(LastCursorPosition.X, LastCursorPosition.Y);
		}
	}
}
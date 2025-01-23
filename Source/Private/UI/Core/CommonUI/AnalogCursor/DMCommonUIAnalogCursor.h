// Copyright CL Chen - All Rights Reserved.

#pragma once

#include <Input/CommonAnalogCursor.h>
#include <Input/CommonUIInputSettings.h>

class UCommonUIActionRouterBase;
class UWidget;

class FDMCommonAnalogCursor : public FCommonAnalogCursor
{
public:
	FDMCommonAnalogCursor(const UCommonUIActionRouterBase& InActionRouter);

	// Begin FCommonAnalogCursor override
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;
	virtual bool ShouldVirtualAcceptSimulateMouseButton(const FKeyEvent& InKeyEvent, EInputEvent InputEvent) const override;
	// End FCommonAnalogCursor override

	// Begin FAnalogCursor override
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
	// End FAnalogCursor override

	void UpdateForceHiddenSwitchOfAnalogCursor(const bool bIsForceHidden);

	virtual float GetStickySlowdown() const;
	virtual void ResetStickySlowdown();

	void SetCursorScale(const float Scale, const bool bIsOnlyForAnalogCursorMode = true);
	void ResetCursorScale();

protected:
	// Begin FCommonAnalogCursor override
	virtual void RefreshCursorSettings() override;

	virtual void HandleInputMethodChanged(ECommonInputType NewInputMethod) override;
	// End FCommonAnalogCursor override

	void HandleAnalogCursorStateByPressedKey(const FKey& PressedKey);
	void EnableAnalogCursor();
	void DisableAnalogCursor();
	void EnableAnalogStickNavigation();

private:
	bool IsVirtualCursorOverriden() const;

	UWidget* GetCursorWidget();
	void ApplyCursorScale();
	void UpdateLastCursorPosition();

	TWeakObjectPtr<UWidget> CachedCursorWidget = nullptr;

	FCommonAnalogCursorSettings CachedAnalogCursorSettings{};

	bool HasSetAnalogCursorSetting = false;

	float CurrentCursorScale = 1.0f;

	bool bIsAnalogNavigationEnabled = false;
	bool bIsAnalogForcedHidden = false;
	bool bPreIsAnalogMovementEnabled = false;
	bool bEnableAnalogMovementAfterForceHidden = false;

	FVector2D LastCursorPosition = { 0.0f, 0.0f };
};
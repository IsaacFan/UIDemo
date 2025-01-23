// Copyright CL Chen - All Rights Reserved.

#pragma once

#include <GameFramework/GameUserSettings.h>

#include "DMGameUserSettings.generated.h"

UCLASS(config = GameUserSettings, configdonotcheckdefaults, BlueprintType, Blueprintable)
class /*UIDEMO_API*/ UDMGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
		FULYGameUserSettingsUpdated,
		const UDMGameUserSettings*, UserSettings);

	// Begin UGameUserSettings override
	void SetToDefaults() override;
	void LoadSettings(bool bForceReload = false) override;
	void ApplySettings(bool bCheckForCommandLineOverrides) override;
	// End UGameUserSettings override

	float GetMouseSensitivity() const;
	void SetMouseSensitivity(float InSensitivity);

	float GetMouseADSSensitivityModifier() const;
	void SetMouseADSSensitivityModifier(float InSensitivity);

	float GetGamepadSensitivityX() const;
	void SetGamepadSensitivityX(float InSensitivity);

	float GetGamepadSensitivityY() const;
	void SetGamepadSensitivityY(float InSensitivity);

	float GetGamepadADSSensitivityModifierX() const;
	void SetGamepadADSSensitivityModifierX(float InSensitivity);

	float GetGamepadADSSensitivityModifierY() const;
	void SetGamepadADSSensitivityModifierY(float InSensitivity);

	/*Misc Settings*/
	float GetSafeZonePercentage() const;
	bool SetSafeZonePercentage(UObject* const WorldContextObject, float InPercentage);

	UPROPERTY(BlueprintAssignable)
	FULYGameUserSettingsUpdated OnGameUserSettingsUpdated;

	UFUNCTION(BlueprintCallable)
	const FString& GetDeviceProfile() const;
	UFUNCTION(BlueprintCallable)
	void SetDeviceProfile(FString Name);

	TArray<FString> GetSelectableDeviceProfiles() const;
	void ActivateDeviceProfile() const;
	void ChooseDeviceProfile();

protected:
	UPROPERTY(config)
	FString DeviceProfileName;

	UPROPERTY(config)
	float MouseSensitivity;

	UPROPERTY(config)
	float MouseADSSensitivityModifier;

	UPROPERTY(config)
	float GamepadSensitivityX;

	UPROPERTY(config)
	float GamepadSensitivityY;	

	UPROPERTY(config)
	float GamepadADSSensitivityModifierX;

	UPROPERTY(config)
	float GamepadADSSensitivityModifierY;

	/*Misc Settings*/
	UPROPERTY(config)
	float SafeZonePercentage = 1.f;

	const float MinSafeZonePercentage = 0.95f;
	const float MaxSafeZonePercentage = 1.f;
};

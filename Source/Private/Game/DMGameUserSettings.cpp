// Copyright CL Chen - All Rights Reserved.

#include "Game/DMGameUserSettings.h"

#include <CommonInputSubsystem.h>
#include <GameplayTagContainer.h>
#include <DeviceProfiles/DeviceProfile.h>
#include <DeviceProfiles/DeviceProfileManager.h>
#include <Engine/World.h>
#include <Misc/ConfigCacheIni.h>

#include "Game/DMGameUserSettingConstants.h"
#include "UI/Event/DMUISceneEvents.h"

void UDMGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();

	MouseSensitivity = DMGameUserSettingConstants::DefaultMouseSensitivity;
	MouseADSSensitivityModifier = DMGameUserSettingConstants::DefaultMouseADSSensitivityModifier;
	GamepadSensitivityX = DMGameUserSettingConstants::DefaultGamepadSensitivityX;
	GamepadSensitivityY = DMGameUserSettingConstants::DefaultGamepadSensitivityY;
	GamepadADSSensitivityModifierX = DMGameUserSettingConstants::DefaultGamepadADSSensitivityModifierX;
	GamepadADSSensitivityModifierY = DMGameUserSettingConstants::DefaultGamepadADSSensitivityModifierY;
}

void UDMGameUserSettings::LoadSettings(bool bForceReload)
{
	Super::LoadSettings(bForceReload);

	ChooseDeviceProfile();
}

void UDMGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);

	OnGameUserSettingsUpdated.Broadcast(this);
}

float UDMGameUserSettings::GetMouseSensitivity() const
{
	return MouseSensitivity;
}

void UDMGameUserSettings::SetMouseSensitivity(float InSensitivity)
{
	MouseSensitivity = InSensitivity;
}

float UDMGameUserSettings::GetMouseADSSensitivityModifier() const
{
	return MouseADSSensitivityModifier;
}

void UDMGameUserSettings::SetMouseADSSensitivityModifier(float InSensitivity)
{
	MouseADSSensitivityModifier = InSensitivity;
}

float UDMGameUserSettings::GetGamepadSensitivityX() const
{
	return GamepadSensitivityX;
}

void UDMGameUserSettings::SetGamepadSensitivityX(float InSensitivity)
{
	GamepadSensitivityX = InSensitivity;
}

float UDMGameUserSettings::GetGamepadSensitivityY() const
{
	return GamepadSensitivityY;
}

void UDMGameUserSettings::SetGamepadSensitivityY(float InSensitivity)
{
	GamepadSensitivityY = InSensitivity;
}

float UDMGameUserSettings::GetGamepadADSSensitivityModifierX() const
{
	return GamepadADSSensitivityModifierX;
}

void UDMGameUserSettings::SetGamepadADSSensitivityModifierX(float InSensitivity)
{
	GamepadADSSensitivityModifierX = InSensitivity;
}

float UDMGameUserSettings::GetGamepadADSSensitivityModifierY() const
{
	return GamepadADSSensitivityModifierY;
}

void UDMGameUserSettings::SetGamepadADSSensitivityModifierY(float InSensitivity)
{
	GamepadADSSensitivityModifierY = InSensitivity;
}

float UDMGameUserSettings::GetSafeZonePercentage() const
{
	return SafeZonePercentage;
}

bool UDMGameUserSettings::SetSafeZonePercentage(UObject* const WorldContextObject, float InPercentage)
{
	InPercentage = FMath::Clamp(InPercentage, MinSafeZonePercentage, MaxSafeZonePercentage);
	if (InPercentage != SafeZonePercentage)
	{
		UWorld* const World = IsValid(WorldContextObject) ? WorldContextObject->GetWorld() : nullptr;
		if (IsValid(World))
		{
			SafeZonePercentage = InPercentage;
			/*
			DMUISceneEvents::FOnSafeZonePercentageChangedEvent OnSafeZonePercentageChangedEvent;
			OnSafeZonePercentageChangedEvent.NewSafeZonePercentage = SafeZonePercentage;
			Event::FWorldEventHelpers::Report(*ThisWorld, OnSafeZonePercentageChangedEvent);
			*/
			return true;
		}
	}

	return false;
}

const FString& UDMGameUserSettings::GetDeviceProfile() const
{
	return DeviceProfileName;
}

void UDMGameUserSettings::SetDeviceProfile(FString Name)
{
	DeviceProfileName = Name;
}

TArray<FString> UDMGameUserSettings::GetSelectableDeviceProfiles() const
{
	FString PlatformName = FPlatformProperties::IniPlatformName();

	TArray<FString> DeviceProfiles;
	FConfigFile* PlatformConfigFile = GConfig->Find(GDeviceProfilesIni);

	TArray<FString> DeviceProfileNameAndTypes;
	PlatformConfigFile->GetArray(TEXT("DeviceProfiles"), TEXT("DeviceProfileNameAndTypes"), DeviceProfileNameAndTypes);

	for (const FString& Desc : DeviceProfileNameAndTypes)
	{
		FString Name, DeviceType;
		Desc.Split(TEXT(","), &Name, &DeviceType);
		const FConfigSection* Section = PlatformConfigFile->FindSection(Name + TEXT(" DeviceProfile"));
		if (DeviceType == PlatformName && Section)
		{
			const FConfigValue* BaseProfileNameValue = Section->Find(TEXT("BaseProfileName"));
			if (BaseProfileNameValue && BaseProfileNameValue->GetValue() == (PlatformName + TEXT("SettingsBase")))
			{
				DeviceProfiles.Add(Name);
			}
		}
	}

	return DeviceProfiles;
}

void UDMGameUserSettings::ActivateDeviceProfile() const
{
	if (DeviceProfileName.IsEmpty() || UDeviceProfileManager::Get().GetActiveDeviceProfileName() == DeviceProfileName)
	{
		return;
	}

	UDeviceProfile* NewActiveProfile = UDeviceProfileManager::Get().FindProfile(DeviceProfileName, true);

	if (NewActiveProfile)
	{
		UDeviceProfileManager::Get().SetOverrideDeviceProfile(NewActiveProfile);
	}
	else
	{
		//UE_LOG(LogDMUI, Warning, TEXT("Failed trying to set device profile: %s"), *DeviceProfileName);
	}
}

void UDMGameUserSettings::ChooseDeviceProfile()
{
	ActivateDeviceProfile();
}


// Copyright CL Chen - All Rights Reserved.

#pragma once

#include <Engine/DeveloperSettings.h>

#include <GameplayTagContainer.h>

#include "DMCommonUISettings.generated.h"

UCLASS(BlueprintType, NotBlueprintable, Config = Game, DefaultConfig, DisplayName="Demo Common UI Settings")
class UDMCommonUISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static UDMCommonUISettings& Get();

	bool IsVirtualCursorOverrideEnabled() const;
	const TArray<FGameplayTag>& GetOverrideVirtualCursorSceneTags() const;
	bool IsSceneTagInVirtualCursorOverrides(const FGameplayTag& SceneTag) const;

private:
	// For safety so we can turn off the overriding behaviour if unexpected issues arise for custom analog cursor behavior.
	UPROPERTY(Config, EditDefaultsOnly, Category = "CommonUI|AnalogCursor")
	bool bIsVirtualCursorOverrideEnabled = false;

	// For granularity we can add scenes over time to this list as there are still many scenes that need the default behavior - it may make sense to add a toggle to full enable across the board eventually.
	UPROPERTY(Config, EditDefaultsOnly,Category = "CommonUI|AnalogCursor", Meta = (Categories = "UI.Scene."))
	TArray<FGameplayTag> OverrideVirtualCursorSceneTags;
};
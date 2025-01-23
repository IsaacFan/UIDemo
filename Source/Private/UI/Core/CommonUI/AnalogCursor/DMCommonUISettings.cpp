// Copyright CL Chen - All Rights Reserved.

#include "UI/Core/CommonUI/DMCommonUISettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DMCommonUISettings)

UDMCommonUISettings& UDMCommonUISettings::Get()
{
	UDMCommonUISettings* DefaultObject = GetMutableDefault<UDMCommonUISettings>();
	check(DefaultObject);
	return *DefaultObject;
}

bool UDMCommonUISettings::IsVirtualCursorOverrideEnabled() const
{
	return bIsVirtualCursorOverrideEnabled;
}

const TArray<FGameplayTag>& UDMCommonUISettings::GetOverrideVirtualCursorSceneTags() const
{
	return OverrideVirtualCursorSceneTags;
}

bool UDMCommonUISettings::IsSceneTagInVirtualCursorOverrides(const FGameplayTag& SceneTag) const
{
	return OverrideVirtualCursorSceneTags.Contains(SceneTag);
}

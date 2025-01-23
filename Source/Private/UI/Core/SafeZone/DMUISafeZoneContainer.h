// Copyright CL Chen - All Rights Reserved.

#pragma once

#include <Components/CanvasPanel.h>
#include "UI/Event/DMUISceneEvents.h"

#include "DMUISafeZoneContainer.generated.h"

class UImage;


namespace DMUISceneEvents
{
	struct FOnSafeZonePercentageChangedEvent;

#if !UE_BUILD_SHIPPING
	struct FSwitchSafeZoneVisibilityEvent;
#endif //!UE_BUILD_SHIPPING
}    // namespace DMUISceneEvents

UCLASS()
class UDMUISafeZoneContainer : public UCanvasPanel
{
	GENERATED_BODY()

public:
	void UpdateSafeZoneScope();

	void RegisterEvents();
	void UnregisterEvents();

private:
	void OnUpdateSafeZonePercentageCallback(
		const DMUISceneEvents::FOnSafeZonePercentageChangedEvent& Event);
#if !UE_BUILD_SHIPPING
	void SwitchSafeZoneVisibilityCallback(
		const DMUISceneEvents::FSwitchSafeZoneVisibilityEvent& Event);
#endif //!UE_BUILD_SHIPPING
	
	//Event::FEventDelegateHandle UpdateSafeZonePercentageHandle;
	
	UPROPERTY(Transient)
	TObjectPtr<UImage> SafeZoneDebugBlockImage = nullptr;
/*
#if !UE_BUILD_SHIPPING
	Event::FEventDelegateHandle SwitchSafeZoneVisibilityHandle;
#endif //!UE_BUILD_SHIPPING
*/
};
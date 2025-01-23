// Copyright CL Chen - All Rights Reserved.

#pragma once

#include <Blueprint/UserWidgetPool.h>

#include "DMUserWidgetPool.generated.h"

/**
 * Wrapper for FUserWidget pool allowing us to inject additional operations when operating on widgets in pool.
 */
USTRUCT()
struct FDMUserWidgetPool
{
	GENERATED_BODY()

public:
	void CreateAndInitializeWidgetPool(UWidget& InOwningWidget);
	void CreateAndInitializeWidgetPool(UWorld* const OwningWorld);
	bool IsInitialized() const;
	void ReleaseSlateResources();

	template <typename UserWidgetT = UUserWidget>
	UserWidgetT* GetOrCreateInstance(TSubclassOf<UserWidgetT> WidgetClass);
	const TArray<UUserWidget*>& GetActiveWidgets() const;

	void Release(UUserWidget* const Widget, bool bReleaseSlate = false);
	void Release(const TArray<UUserWidget*>& Widgets, bool bReleaseSlate = false);
	void ReleaseAll(const bool bReleaseSlate = false);

	void ResetPool();

private:
	UPROPERTY(Transient)
	FUserWidgetPool UserWidgetPool = {};
};
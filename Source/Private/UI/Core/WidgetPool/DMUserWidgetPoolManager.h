// Copyright CL Chen - All Rights Reserved.

#pragma once

#include <GameplayTagContainer.h>

#include "UI/Core/WidgetPool/DMUserWidgetPool.h"

#include "DMUserWidgetPoolManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDMUIWidgetPoolManager, Log, All);

/**
 * Allows management of categorized widget pools that need to extend across multiple UI contexts
 * Objects wanting to use widgets placed in those pools must implement IULYGlobalWidgetPoolConsumer
 * in order to ensure widgets are correctly released back to pools.
 */
UCLASS()
class /*DEMO_API*/ UDMUserWidgetPoolManager : public UObject
{
	GENERATED_BODY()

public:
	// Gets or creates a new widget from associated widget pool, if the pool does not exist it will also be created
	template <typename TUserWidget = UUserWidget>
	TUserWidget* GetOrCreateInstance(
		const FGameplayTag InPoolCategory,
		TSubclassOf<TUserWidget> WidgetClass
	);

	void ReleaseWidget(const FGameplayTag InPoolCategory, UUserWidget* const InUserWidget, bool bReleaseSlate = false);
	void ReleaseWidgets(const FGameplayTag InPoolCategory, const TArray<UUserWidget*>& InUserWidgets, bool bReleaseSlate = false);

	void ResetPool(const FGameplayTag& InPoolCategory);
	void ResetAllPools();

	FString GetPoolsStateOutput() const;

private:
	FDMUserWidgetPool* CreateOrFindPool(const FGameplayTag& InPoolType);

	UPROPERTY(Transient)
	TMap<FGameplayTag, FDMUserWidgetPool> WidgetPools;
};




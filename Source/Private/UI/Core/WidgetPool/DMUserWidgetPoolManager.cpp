// Copyright CL Chen - All Rights Reserved.

#include "UI/Core/WidgetPool/DMUserWidgetPoolManager.h"

DEFINE_LOG_CATEGORY(LogDMUIWidgetPoolManager);

template<typename TUserWidget>
TUserWidget* UDMUserWidgetPoolManager::GetOrCreateInstance(
	const FGameplayTag InPoolCategory,
	TSubclassOf<TUserWidget> WidgetClass)
{
	FDMUserWidgetPool* const TargetPool = CreateOrFindPool(InPoolCategory);
	if (!TargetPool)
	{
		UE_LOG(
			LogDMUIWidgetPoolManager,
			Error,
			TEXT("%hs - Failed to find or create a pool for the supplied type"),
			__FUNCTION__);
		return nullptr;
	}

	auto* const Widget = Cast<TUserWidget>(TargetPool->GetOrCreateInstance(WidgetClass));

	UE_LOG(
		LogDMUIWidgetPoolManager,
		Verbose,
		TEXT("%hs - Widget obtained from widget pool %s. Active widgets: %i"),
		__FUNCTION__,
		*InPoolCategory.ToString(),
		TargetPool->GetActiveWidgets().Num());

	return Widget;
}

void UDMUserWidgetPoolManager::ReleaseWidget(
	const FGameplayTag InPoolCategory,
	UUserWidget* const InUserWidget,
	bool bReleaseSlate /*= false*/)
{
	FDMUserWidgetPool* const FoundPool = WidgetPools.Find(InPoolCategory);
	if (!FoundPool)
	{
		return;
	}

	FoundPool->Release(InUserWidget, bReleaseSlate);

	UE_LOG(
		LogDMUIWidgetPoolManager,
		Verbose,
		TEXT("%hs - Widget released from global pool %s. Active widgets: %i"),
		__FUNCTION__,
		*InPoolCategory.ToString(),
		FoundPool->GetActiveWidgets().Num());
}

void UDMUserWidgetPoolManager::ReleaseWidgets(
	const FGameplayTag InPoolCategory,
	const TArray<UUserWidget*>& InUserWidgets,
	bool bReleaseSlate /*= false*/)
{
	FDMUserWidgetPool* const FoundPool = WidgetPools.Find(InPoolCategory);
	if (!FoundPool)
	{
		return;
	}

	FoundPool->Release(InUserWidgets, bReleaseSlate);
}

void UDMUserWidgetPoolManager::ResetPool(const FGameplayTag& InPoolCategory)
{
	FDMUserWidgetPool* const FoundPool = WidgetPools.Find(InPoolCategory);
	if (!FoundPool)
	{
		return;
	}

	FoundPool->ReleaseAll(true);
	WidgetPools.Remove(InPoolCategory);

	UE_LOG(
		LogDMUIWidgetPoolManager,
		Verbose,
		TEXT("%hs - Widgets released from global pool %s. Active widgets: %i"),
		__FUNCTION__,
		*InPoolCategory.ToString(),
		FoundPool->GetActiveWidgets().Num());
}

void UDMUserWidgetPoolManager::ResetAllPools()
{
	TArray<FGameplayTag> PoolKeys;
	WidgetPools.GetKeys(PoolKeys);
	for (const FGameplayTag& PoolKey : PoolKeys)
	{
		ResetPool(PoolKey);
	}

	UE_LOG(
		LogDMUIWidgetPoolManager,
		Verbose,
		TEXT("%hs - All global widget pools reset"),
		__FUNCTION__);
}

FString UDMUserWidgetPoolManager::GetPoolsStateOutput() const
{
	FString Output;
	for (const auto& WidgetPoolEntry : WidgetPools)
	{
		FGameplayTag PoolTag = WidgetPoolEntry.Key;
		Output.Append(TEXT("* Widget Pool Tag: "));
		Output.Append(PoolTag.ToString());

		const FDMUserWidgetPool WidgetPool = WidgetPoolEntry.Value;
		if (!WidgetPool.IsInitialized())
		{
			Output.Append(TEXT("The pool is not initialized"));
			continue;
		}

		Output.Append(TEXT(" | Active widgets in pool: "));
		Output.Append(FString::FormatAsNumber(WidgetPool.GetActiveWidgets().Num()));
		Output.Append(TEXT("\n"));
	}

	return Output;
}

FDMUserWidgetPool* UDMUserWidgetPoolManager::CreateOrFindPool(const FGameplayTag& InPoolType)
{
	FDMUserWidgetPool* TargetPool = WidgetPools.Find(InPoolType);
	if (!TargetPool)
	{
		UWorld* const World = GetWorld();
		if (!ensureMsgf(
				World,
				TEXT("%s - Widget Pool Manager lacks a valid World and will not be able to create functional widget pools"),
				ANSI_TO_TCHAR(__FUNCTION__)))
		{
			return nullptr;
		}

		TargetPool = &WidgetPools.Emplace(InPoolType);
		if (!TargetPool)
		{
			return nullptr;
		}

		TargetPool->CreateAndInitializeWidgetPool(World);

		UE_LOG(
			LogDMUIWidgetPoolManager,
			Verbose,
			TEXT("%hs - Created new global user widget pool %s"),
			__FUNCTION__,
			*InPoolType.ToString());
	}

	return TargetPool;
}

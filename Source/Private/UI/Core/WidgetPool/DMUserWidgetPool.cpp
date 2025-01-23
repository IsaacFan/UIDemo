// Copyright CL Chen - All Rights Reserved.

#include "UI/Core/WidgetPool/DMUserWidgetPool.h"

#include "UI/Core/DMUserWidgetInterface.h"

void FDMUserWidgetPool::CreateAndInitializeWidgetPool(UWidget& InOwningWidget)
{
	UserWidgetPool = FUserWidgetPool(InOwningWidget);
}

void FDMUserWidgetPool::CreateAndInitializeWidgetPool(UWorld* const OwningWorld)
{
	UserWidgetPool = FUserWidgetPool();
	UserWidgetPool.SetWorld(OwningWorld);
}

bool FDMUserWidgetPool::IsInitialized() const
{
	return UserWidgetPool.IsInitialized();
}

void FDMUserWidgetPool::ReleaseSlateResources()
{
	UserWidgetPool.ReleaseAllSlateResources();
}

template<typename UserWidgetT>
UserWidgetT* FDMUserWidgetPool::GetOrCreateInstance(TSubclassOf<UserWidgetT> WidgetClass)
{
	if (!UserWidgetPool.IsInitialized())
	{
		/*UE_LOG(
			LogDMUI,
			Error,
			TEXT("%hs - Widgets cannot be created in uninitialized widget pool"),
			__FUNCTION__
		);*/
		return nullptr;
	}

	return UserWidgetPool.GetOrCreateInstance(WidgetClass);
}

const TArray<UUserWidget*>& FDMUserWidgetPool::GetActiveWidgets() const
{
	return UserWidgetPool.GetActiveWidgets();
}

void FDMUserWidgetPool::Release(UUserWidget* const Widget, const bool bReleaseSlate/* = false*/)
{
	// For our custom user widgets we want to ensure they have disconnected from all dependencies
	// that can cause them to try and update while inactive in the pool.
	if (auto* const UserWidgetInterface = Cast<IDMUserWidgetInterface>(Widget))
	{
		UserWidgetInterface->OnReleasedToWidgetPool();
	}

	UserWidgetPool.Release(Widget, bReleaseSlate);
}

void FDMUserWidgetPool::Release(const TArray<UUserWidget*>& Widgets, bool bReleaseSlate)
{
	for (UUserWidget* const Widget : Widgets)
	{
		Release(Widget, bReleaseSlate);
	}
}

void FDMUserWidgetPool::ReleaseAll(const bool bReleaseSlate)
{
	UserWidgetPool.ReleaseAll(bReleaseSlate);
}

void FDMUserWidgetPool::ResetPool()
{
	UserWidgetPool.ResetPool();
}

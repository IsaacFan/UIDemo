// Copyright CL Chen - All Rights Reserved.

#include "UI/Core/ViewModelManagement/DMViewModelManager.h"

#include <Engine/World.h>
#include <TimerManager.h>

DEFINE_LOG_CATEGORY(LogDMViewModelManager);

void UDMViewModelManager::Initialize()
{
	UWorld* const World = GetWorld();
	if (IsValid(World))
	{
		World->GetTimerManager().SetTimer(
			StaleModelCheckingTimerHandle,
			this,
			&UDMViewModelManager::RemoveStaleModelReferences,
			StaleModelCheckingInterval,
			true);
	}
}

template<class T>
T* UDMViewModelManager::GetOrCreateViewModel(
	UObject* const ModelObject,
	TSubclassOf<UDMViewModel> ViewModelClass,
	const bool MarkAsPersistent /* = false*/)
{
	FReferredModel ReferredModel(ModelObject, ViewModelClass);
	UDMViewModel* const ExistingViewModel = GetExistingViewModel(ReferredModel, MarkAsPersistent);
	if (IsValid(ExistingViewModel))
	{
		return Cast<T>(ExistingViewModel);
	}

	UDMViewModel* const NewViewModel = NewObject<UDMViewModel>(this, ViewModelClass);
	NewViewModel->OnViewModelDestroyedDelegate.BindUObject(
		this,
		&UDMViewModelManager::OnViewModelDestroyed);
	if (MarkAsPersistent == true)
	{
		PersistentViewModelMap.Add(ReferredModel, NewViewModel);
	}
	else
	{
		InconstantViewModelMap.Add(ReferredModel, NewViewModel);
	}

	return Cast<T>(NewViewModel);
}

void UDMViewModelManager::OnViewModelDestroyed(const UDMViewModel* const ViewModel)
{
	for (const auto& Pair : InconstantViewModelMap)
	{
		if (Pair.Value == ViewModel)
		{
			InconstantViewModelMap.Remove(Pair.Key);
			break;
		}
	}

	for (const auto& Pair : PersistentViewModelMap)
	{
		if (Pair.Value == ViewModel)
		{
			PersistentViewModelMap.Remove(Pair.Key);
			if (IsValid(ViewModel))
			{
				UE_LOG(
					LogDMViewModelManager,
					Error,
					TEXT("%s - The ViewModel %s was marked as persistent but is destroyed."),
					ANSI_TO_TCHAR(__FUNCTION__),
					*ViewModel->GetName());
			}
			break;
		}
	}
}

UDMViewModel* UDMViewModelManager::GetExistingViewModel(
	FReferredModel& ReferredModel, 
	const bool MarkAsPersistent)
{
	TObjectPtr<UDMViewModel>* const ExistingPersistentViewModel = PersistentViewModelMap.Find(ReferredModel);
	if (ExistingPersistentViewModel != nullptr)
	{
		return *ExistingPersistentViewModel;
	}

	TWeakObjectPtr<UDMViewModel>* const ExistingInconstantViewModel = 
		InconstantViewModelMap.Find(ReferredModel);
	UDMViewModel* const ExistingViewModel = ExistingInconstantViewModel ? 
		ExistingInconstantViewModel->Get() : 
		nullptr;
	if (IsValid(ExistingViewModel))
	{
		if (MarkAsPersistent == true)
		{
			InconstantViewModelMap.Remove(ReferredModel);
			PersistentViewModelMap.Add(ReferredModel, ExistingViewModel);
		}

		return ExistingViewModel;
	}

	return nullptr;
}

void UDMViewModelManager::RemoveStaleModelReferences()
{
	TArray<FReferredModel> PersistentViewModelMapKeys;
	PersistentViewModelMap.GenerateKeyArray(PersistentViewModelMapKeys);
	for (const FReferredModel& ReferredModel : PersistentViewModelMapKeys)
	{
		if (ReferredModel.GetModelObject() == nullptr)
		{
			PersistentViewModelMap.Remove(ReferredModel);
		}
	}

	TArray<FReferredModel> InconstantViewModelMapKeys;
	InconstantViewModelMap.GenerateKeyArray(InconstantViewModelMapKeys);
	for (const FReferredModel& ReferredModel : InconstantViewModelMapKeys)
	{
		if (ReferredModel.GetModelObject() == nullptr)
		{
			InconstantViewModelMap.Remove(ReferredModel);
		}
	}
}
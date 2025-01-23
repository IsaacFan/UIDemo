// Copyright CL Chen - All Rights Reserved.

#pragma once

#include <UObject/Object.h>
#include <Templates/SubclassOf.h>
#include <Engine/EngineTypes.h>

#include "UI/Core/DMViewModel.h"

#include "DMViewModelManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDMViewModelManager, Warning, All);

USTRUCT(BlueprintType)
struct FReferredModel
{
	GENERATED_BODY()

public:
	FReferredModel()
	: ModelObject(nullptr)
	, ViewModelClass(nullptr)
	{}

	FReferredModel(UObject* const InModelObject, TSubclassOf<UDMViewModel> InViewModelClass)
	: ModelObject(InModelObject)
	, ViewModelClass(InViewModelClass)
	{}

	bool operator==(const FReferredModel& Other) const
	{
		return ModelObject.Get() == Other.GetModelObject() &&
			ViewModelClass == Other.GetViewModelClass();
	}

	uint32 GetHash() const;
	inline friend uint32 GetTypeHash(const FReferredModel& ReferredModel)
	{
		return ReferredModel.GetHash();
	}

	UObject* GetModelObject() const;
	TSubclassOf<UDMViewModel> GetViewModelClass() const;

private:
	UPROPERTY()
	TWeakObjectPtr<UObject> ModelObject = nullptr;

	UPROPERTY()
	TSubclassOf<UDMViewModel> ViewModelClass = nullptr;
};

inline uint32 FReferredModel::GetHash() const
{
	return HashCombine(GetTypeHash(ModelObject.Get()), GetTypeHash(ViewModelClass));
}

inline UObject* FReferredModel::GetModelObject() const
{
	return ModelObject.Get();
}

inline TSubclassOf<UDMViewModel> FReferredModel::GetViewModelClass() const
{
	return ViewModelClass;
}


UCLASS()
class UDMViewModelManager : public UObject
{
	GENERATED_BODY()

public:
	void Initialize();

	template<class T>
	T* GetOrCreateViewModel(
		UObject* const ModelObject, 
		TSubclassOf<UDMViewModel> ViewModelClass,
		const bool AlwaysKeepTheViewModel = false);

private:
	void OnViewModelDestroyed(const UDMViewModel* const ViewModel);

	void RemoveStaleModelReferences();

	UDMViewModel* GetExistingViewModel(FReferredModel& ReferredModel, const bool MarkAsPersistent);


	UPROPERTY(Transient)
	TMap<FReferredModel, TObjectPtr<UDMViewModel>> PersistentViewModelMap;

	UPROPERTY(Transient)
	TMap<FReferredModel, TWeakObjectPtr<UDMViewModel>> InconstantViewModelMap;

	FTimerHandle StaleModelCheckingTimerHandle;
	const float StaleModelCheckingInterval = 120.f;

};

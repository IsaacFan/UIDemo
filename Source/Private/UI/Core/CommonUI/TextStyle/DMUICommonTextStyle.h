// Copyright CL Chen - All Rights Reserved.

#pragma once

#include <CommonTextBlock.h>
#include <GameplayTagContainer.h>

#include "DMUICommonTextStyle.generated.h"

UCLASS(Abstract, Blueprintable, ClassGroup = UI, meta = (Category = "Common UI"))
class UDMUICommonTextStyle : public UCommonTextStyle
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Font", Meta = (Categories = "UI.Font"))
	FGameplayTag FontScalingTag = FGameplayTag::EmptyTag;
};
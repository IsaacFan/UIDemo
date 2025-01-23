// Copyright CL Chen - All Rights Reserved.

#pragma once

#include <Input/CommonUIActionRouterBase.h>

#include "DMCommonUIActionRouter.generated.h"

UCLASS()
class UDMCommonUIActionRouter : public UCommonUIActionRouterBase
{
	GENERATED_BODY()

protected:
	// Begin UCommonUIActionRouterBase override
	virtual TSharedRef<FCommonAnalogCursor> MakeAnalogCursor() const override;
	// End UCommonUIActionRouterBase override
};
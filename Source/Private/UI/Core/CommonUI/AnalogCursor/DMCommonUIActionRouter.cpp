// Copyright CL Chen - All Rights Reserved.

#include "UI/Core/CommonUI/DMCommonUIActionRouter.h"

#include "UI/Core/CommonUI/DMCommonUIAnalogCursor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DMCommonUIActionRouter)

TSharedRef<FCommonAnalogCursor> UDMCommonUIActionRouter::MakeAnalogCursor() const
{
	return FCommonAnalogCursor::CreateAnalogCursor<FDMCommonAnalogCursor>(*this);
}

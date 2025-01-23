// Copyright CL Chen - All Rights Reserved.

#pragma once

#include <Net/Core/Connection/NetEnums.h>
#include <UObject/Object.h>

#include "DMUINetworkHandler.generated.h"

class UNetDriver;
class UWorld;

/**
 * Class responsible for any UI related network stuff.
 * Like disconnected from the server, received any messages from the server, 
 * or "push" notifications that players receive while offline.
 */
UCLASS()
class UDMUINetworkHandler : public UObject
{
	GENERATED_BODY()

	enum class ENetworkMessage: uint8
	{
		DisconnectedFromServer,
	};

public:
	void Initialize();

	static void ExecuteQueuedMessage(const UObject* const WorldContextObject);

	UFUNCTION()
	void ShowNextQueuedMessage();

private:
	void HandleNetworkFailure(
		UWorld* const World,
		UNetDriver* const NetDriver,
		const ENetworkFailure::Type FailureType,
		const FString& ErrorString
	);
	void ShowNetworkFailureMessage();

	TArray<ENetworkMessage> NetworkMessageQueue = {};
};
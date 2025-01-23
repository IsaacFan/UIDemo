// Copyright CL Chen - All Rights Reserved.

#include "UI/Core/NetworkManagement/DMUINetworkHandler.h"

#include <Engine/Engine.h>
#include <Engine/World.h>
#include <GameFramework/PlayerController.h>

#include "UI/Core/DMUIGameSubsystem.h"
#include "UI/Helpers/DMUIFunctionLibrary.h"

void UDMUINetworkHandler::Initialize()
{
	check(GEngine);
	GEngine->OnNetworkFailure().RemoveAll(this);
	GEngine->OnNetworkFailure().AddUObject(this, &ThisClass::HandleNetworkFailure);
}

void UDMUINetworkHandler::ExecuteQueuedMessage(const UObject* const WorldContextObject)
{
	const UDMUIGameSubsystem* const UIGameSubsystem = UDMUIFunctionLibrary::GetUIGameSubsystem(WorldContextObject);
	UDMUINetworkHandler* const UINetworkHandler = IsValid(UIGameSubsystem) ? UIGameSubsystem->GetUINetworkHandler() : nullptr;
	if (!IsValid(UINetworkHandler))
	{
		return;
	}

	UINetworkHandler->ShowNextQueuedMessage();
}

void UDMUINetworkHandler::ShowNextQueuedMessage()
{
	if (NetworkMessageQueue.Num() <= 0)
	{
		return;
	}

	ENetworkMessage MessageType;
	NetworkMessageQueue.HeapPop(MessageType);
	switch (MessageType)
	{
	case ENetworkMessage::DisconnectedFromServer:
		ShowNetworkFailureMessage();
		break;
	default:
		break;
	}
}

void UDMUINetworkHandler::HandleNetworkFailure(
	UWorld* const World,
	UNetDriver* const NetDriver,
	const ENetworkFailure::Type FailureType,
	const FString& ErrorString)
{
	if (NetworkMessageQueue.Contains(ENetworkMessage::DisconnectedFromServer))
	{
		return;
	}

	NetworkMessageQueue.Add(ENetworkMessage::DisconnectedFromServer);
}

void UDMUINetworkHandler::ShowNetworkFailureMessage()
{
	UWorld* const World = GetWorld();
	APlayerController* const PlayerController = IsValid(World) ?
		Cast<APlayerController>(World->GetFirstPlayerController()) :
		nullptr;
	if (!IsValid(PlayerController))
	{
		return;
	}

	// TODO: (chunliang.chen@) - Open pop up window and display the nextwork failure message. 
	//                           When pop up window is closed, trigger the ShowNextQueuedMessage().
	/*
	auto* PopupSceneParams = NewObject<UDMUIPopupSceneViewModelParams>();
	PopupSceneParams->InitializeParams(PlayerController->GetPawn());

	DMUIFunctionLibrary::LoadAndOpenPopupAsync(
		DMUIPopupNames::DisconnectedFromServer,
		PlayerController,
		PopupSceneParams)
	.Then(this, [this](UDMUIPopupViewModel* PopupViewModel)
	{
		if(PopupViewModel)
		{
			PopupViewModel->ConfirmButtonClickedDelegate.AddDynamic(this, &ThisClass::ShowNextQueuedMessage);
			PopupViewModel->CancelButtonClickedDelegate.AddDynamic(this, &ThisClass::ShowNextQueuedMessage);
		}
	});
	*/
}

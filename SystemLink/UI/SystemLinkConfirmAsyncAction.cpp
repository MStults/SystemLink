#include "SystemLinkConfirmAsyncAction.h"

#include "SystemLink/SystemLink.h"

USystemLinkConfirmAsyncAction* USystemLinkConfirmAsyncAction::ConfirmUsingControllerAsync(
	ASystemLinkPlayerController* Controller,
	const EConfirmScreenType ConfirmScreenType,
	const FString& InScreenTitle,
	const FString& InScreenMsg)
{
	USystemLinkConfirmAsyncAction* Action = NewObject<USystemLinkConfirmAsyncAction>();
	Action->CachedController = Controller;
	Action->CachedType = ConfirmScreenType;
	Action->CachedTitle = InScreenTitle;
	Action->CachedMsg = InScreenMsg;
	return Action;
}

USystemLinkConfirmAsyncAction* USystemLinkConfirmAsyncAction::ConfirmAsync(
	USystemLinkActivatableWidget* ActivatableWidget,
	const EConfirmScreenType ConfirmScreenType,
	const FString& InScreenTitle,
	const FString& InScreenMsg)
{
	USystemLinkConfirmAsyncAction* Action = NewObject<USystemLinkConfirmAsyncAction>();
	Action->CachedController = ActivatableWidget->GetSystemLinkPlayerController();
	Action->CachedType = ConfirmScreenType;
	Action->CachedTitle = InScreenTitle;
	Action->CachedMsg = InScreenMsg;
	return Action;
}

void USystemLinkConfirmAsyncAction::Activate()
{
	if (!CachedController)
	{
		SetReadyToDestroy();
		return;
	}

	// Call into your new ShowModal with a lambda
	CachedController->ShowModal(
		CachedType,
		FText::FromString(CachedTitle),
		FText::FromString(CachedMsg),
		[this](const EConfirmScreenButtonType ButtonType)
		{
			UE_LOG(LogSystemLink, Display, TEXT("CALLING HandleModalClosed"));
			HandleModalClosed(ButtonType);
		}
	);
}

void USystemLinkConfirmAsyncAction::HandleModalClosed(EConfirmScreenButtonType ButtonType)
{
	// Enhanced log statement to include the button type
	UE_LOG(LogSystemLink, Display, TEXT("CALLING Completed Broadcast with ButtonType: %d"), static_cast<int32>(ButtonType));
    

	Completed.Broadcast(ButtonType);
	SetReadyToDestroy();
}

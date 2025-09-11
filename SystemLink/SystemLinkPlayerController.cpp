// Fill out your copyright notice in the Description page of Project Settings.


#include "SystemLinkPlayerController.h"

#include "CommonInputSubsystem.h"
#include "SystemLink.h"
#include "UI/SystemLinkActivatableWidget.h"
#include "UI/SystemLinkConfirmModal.h"
#include "GameFramework/Pawn.h"
#include "UI/SystemLinkConfirmAsyncAction.h"

USystemLinkActivatableWidget* ASystemLinkPlayerController::PushToStack_Internal(
	const TSubclassOf<class USystemLinkActivatableWidget>& WidgetClass, UCommonActivatableWidgetStack* Stack)
{
	if (!Stack)
	{
		checkf(false, TEXT("Stack was null when trying to push a widget. Make sure UIRootContainer is initialized."));
		return nullptr;
	}

	if (!WidgetClass)
	{
		checkf(false, TEXT("WidgetClass was null in when pushing to stack."));
		return nullptr;
	}

	if (UCommonActivatableWidget* NewWidget = Stack->AddWidget(WidgetClass))
	{
		NewWidget->ActivateWidget();

		USystemLinkActivatableWidget* SystemLinkActivatableWidget = Cast<USystemLinkActivatableWidget>(NewWidget);

		if (!SystemLinkActivatableWidget)
		{
			checkf(false, TEXT("SystemLinkActivatableWidget was null in when pushing to stack."));
			return nullptr;
		}
		
		return SystemLinkActivatableWidget;
	}

	return nullptr;
}

USystemLinkActivatableWidget* ASystemLinkPlayerController::ShowWidget_Internal(
	const TSubclassOf<class USystemLinkActivatableWidget>& WidgetClass, UCommonActivatableWidgetStack* Stack)
{
	// Focus logic
	if (USystemLinkActivatableWidget* TopWidget = PushToStack_Internal(WidgetClass, Stack))
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(TopWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		
		SetInputMode(InputMode);
		bShowMouseCursor = true;

		FString DebugStr = GetCurrentInputModeDebugString();
		UE_LOG(LogTemp, Log, TEXT("Current Input Mode: %s"), *DebugStr);

		if (const auto SystemLinkWidget = Cast<USystemLinkActivatableWidget>(TopWidget))
		{
			// Call TryRestoreFocusIfNeeded on the specific SystemLinkWidget
			SystemLinkWidget->TryRestoreFocusIfNeeded();
			UE_LOG(LogSystemLink, Warning, TEXT("SystemLinkActivatableWidget TryRestoreFocusIfNeeded called!"));
		} else
		{
			UE_LOG(LogSystemLink, Warning, TEXT("Top widget is not a SystemLinkActivatableWidget!"));
			TopWidget->SetFocus();
		}

		return TopWidget;
	}

	return nullptr;
}

void ASystemLinkPlayerController::OnPossess(APawn* PossessPawn)
{
	Super::OnPossess(PossessPawn);

	UIRoot = CreateWidget<USystemLinkUiRootContainer>(this, UIRootClass);

	if (UIRoot)
	{
		UIRoot->AddToPlayerScreen();
		MenuStack = UIRoot->GetMenuStack();
		HudStack = UIRoot->GetHudStack();
		ModalStack = UIRoot->GetModalStack();
		OnUIRootReady(UIRoot);
		StartSystemLinkActivatableWidgetTimer();
	}
}

USystemLinkActivatableWidget* ASystemLinkPlayerController::PushToMenu(
	const TSubclassOf<USystemLinkActivatableWidget> WidgetClass) const
{
	return PushToStack_Internal(WidgetClass, MenuStack);
}

USystemLinkActivatableWidget* ASystemLinkPlayerController::PushToHud(
	const TSubclassOf<USystemLinkActivatableWidget> WidgetClass) const
{
	return PushToStack_Internal(WidgetClass, HudStack);
}

USystemLinkActivatableWidget* ASystemLinkPlayerController::ShowMenu(const TSubclassOf<USystemLinkActivatableWidget> WidgetClass)
{
	return ShowWidget_Internal(WidgetClass, MenuStack);
}

USystemLinkActivatableWidget* ASystemLinkPlayerController::GetActiveWidget() const
{
	UCommonActivatableWidget* ActiveWidget;
	
	if (const auto ModalWidget = ModalStack->GetActiveWidget())
	{
		ActiveWidget = ModalWidget;
	}
	else if (const auto MenuWidget = MenuStack->GetActiveWidget())
	{
		ActiveWidget = MenuWidget;
	}
	else
	{
		ActiveWidget = HudStack->GetActiveWidget();
	}

	return Cast<USystemLinkActivatableWidget>(ActiveWidget);
}

void ASystemLinkPlayerController::ShowModal(
	const EConfirmScreenType ConfirmScreenType,
	const FText& InScreenTitle,
	const FText& InScreenMsg,
	const TFunction<void(EConfirmScreenButtonType)>& ButtonClickedCallback
	) const
{
	const UConfirmScreenInfoObject* CreatedInfoObject = nullptr;
	
	const TSubclassOf<USystemLinkConfirmModal> ConfirmModalClass =
		ConfirmScreenType == EConfirmScreenType::Ok
			? OkModalClass
			: ConfirmScreenType == EConfirmScreenType::YesNo
				? YesNoModalClass
				: OkCancelModalClass;
	
	switch (ConfirmScreenType) {
	case EConfirmScreenType::Ok:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOKScreen(InScreenTitle,InScreenMsg);
		break;
	case EConfirmScreenType::YesNo:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateYesNoScreen(InScreenTitle,InScreenMsg);
		break;
	case EConfirmScreenType::OkCancel:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOkCancelScreen(InScreenTitle,InScreenMsg);
		break;
	case EConfirmScreenType::Unknown:
		break;
	}

	ModalStack->ClearWidgets();
	if (UCommonActivatableWidget* ActivatableWidget = ModalStack->AddWidget(ConfirmModalClass))
	{
		USystemLinkConfirmModal* CreatedConfirmScreen = CastChecked<USystemLinkConfirmModal>(ActivatableWidget);
		CreatedConfirmScreen->InitConfirmScreen(CreatedInfoObject,ButtonClickedCallback);
	}	
}

/**
 * Ensures a safe editor cursor state when this PlayerController ends play
 * (e.g. exiting PIE or unloading a level).
 *
 * Resets:
 * - Shows the OS cursor
 * - Restores Game & UI input mode with no widget focus
 *
 * @param EndPlayReason Reason the actor is being removed from the world.
 */
void ASystemLinkPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ClearSystemLinkActivatableWidgetTimer();	
	
#if WITH_EDITOR
	// Make the editor cursor safe before we die
	if (IsLocalController())
	{
		if (const ULocalPlayer* LocalPlayer = GetLocalPlayer())
		{
			if (UCommonInputSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UCommonInputSubsystem>())
			{
				InputSubsystem->SetCurrentInputType(ECommonInputType::MouseAndKeyboard);
			}
		}
	}
#endif

	Super::EndPlay(EndPlayReason);
}

void ASystemLinkPlayerController::TryRestoreFocusIfNeeded() const
{
	if (const auto ActiveWidget = GetActiveWidget())
	{
		// Check and cast ActiveWidget as USystemLinkActivatableWidget
		if (const auto SystemLinkWidget = Cast<USystemLinkActivatableWidget>(ActiveWidget))
		{
			// Call TryRestoreFocusIfNeeded on the specific SystemLinkWidget
			SystemLinkWidget->TryRestoreFocusIfNeeded();
		}
	}
}

void ASystemLinkPlayerController::StartSystemLinkActivatableWidgetTimer()
{
	// Make sure this runs in a valid world context
	if (GetWorld() && !GetWorld()->GetTimerManager().IsTimerActive(SystemLinkActivatableWidgetTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(
			SystemLinkActivatableWidgetTimerHandle,               
			this,                     
			&ASystemLinkPlayerController::TryRestoreFocusIfNeeded,
			0.2f,                      
			true                        
		);
	}
}

void ASystemLinkPlayerController::ClearSystemLinkActivatableWidgetTimer()
{
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(SystemLinkActivatableWidgetTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(SystemLinkActivatableWidgetTimerHandle);
	}	
}

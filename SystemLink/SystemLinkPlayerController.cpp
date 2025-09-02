// Fill out your copyright notice in the Description page of Project Settings.


#include "SystemLinkPlayerController.h"

#include "SystemLink.h"
#include "UI/SystemLinkActivatableWidget.h"

UCommonActivatableWidget* ASystemLinkPlayerController::PushToStack_Internal(
	const TSubclassOf<class UCommonActivatableWidget>& WidgetClass, UCommonActivatableWidgetStack* Stack)
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
		UE_LOG(LogSystemLink, Warning, TEXT("ActivateWidget called!"));
		return NewWidget;
	}

	return nullptr;
}

UCommonActivatableWidget* ASystemLinkPlayerController::ShowWidget_Internal(
	const TSubclassOf<class UCommonActivatableWidget>& WidgetClass, UCommonActivatableWidgetStack* Stack)
{
	// Focus logic
	if (UCommonActivatableWidget* TopWidget = PushToStack_Internal(WidgetClass, Stack))
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

void ASystemLinkPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Create and add the UI root widget (that holds both stacks)
	if (UIRootClass)
	{
		UIRoot = CreateWidget<USystemLinkUiRootContainer>(this, UIRootClass);

		if (UIRoot)
		{
			UIRoot->AddToViewport();
			MenuStack = UIRoot->GetMenuStack();
			HudStack = UIRoot->GetHudStack();
			ModalStack = UIRoot->GetModalStack();
			OnUIRootReady(UIRoot);
			StartSystemLinkActivatableWidgetTimer();
		}
	}
}

UCommonActivatableWidget* ASystemLinkPlayerController::PushToMenu_Implementation(
	const TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
	return PushToStack_Internal(WidgetClass, MenuStack);
}

UCommonActivatableWidget* ASystemLinkPlayerController::PushToHud_Implementation(
	TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
	return PushToStack_Internal(WidgetClass, HudStack);
}

UCommonActivatableWidget* ASystemLinkPlayerController::ShowMenu_Implementation(
	const TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
	return ShowWidget_Internal(WidgetClass, MenuStack);
}

UCommonActivatableWidget* ASystemLinkPlayerController::ShowModal_Implementation(
	const TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
	return ShowWidget_Internal(WidgetClass, ModalStack);
}

UCommonActivatableWidget* ASystemLinkPlayerController::GetActiveWidget_Implementation() const
{
	if (UCommonActivatableWidget* Widget = ModalStack->GetActiveWidget())
	{
		return Widget;
	}
	
	if (UCommonActivatableWidget* Widget = MenuStack->GetActiveWidget())
	{
		return Widget;		
	}
	
	return HudStack->GetActiveWidget();
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

	Super::EndPlay(EndPlayReason);
	
	// make the editor cursor safe before we die
	bShowMouseCursor = true;
	
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;	
}

void ASystemLinkPlayerController::TryRestoreFocusIfNeeded() const
{
	if (const auto ActiveWidget = GetActiveWidget_Implementation())
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

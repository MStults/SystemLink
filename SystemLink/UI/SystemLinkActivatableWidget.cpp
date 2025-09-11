#include "SystemLinkActivatableWidget.h"

#include "CommonButtonBase.h"
#include "CommonInputSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Widget.h"
#include "Framework/Application/SlateApplication.h"
#include "SystemLink/SystemLink.h"
#include "SystemLink/SystemLinkPlayerController.h"

namespace
{
	// Returns true if the keyboard focus is currently on a Slate/UMG editable text widget.
	static inline bool IsAnyTextEntryActive()
	{
		if (!FSlateApplication::IsInitialized())
		{
			return false;
		}

		TSharedPtr<SWidget> Focused = FSlateApplication::Get().GetKeyboardFocusedWidget();
		if (!Focused.IsValid())
		{
			return false;
		}

		// Heuristics that work across engine versions without private headers:
		// We treat any of the standard Slate text widgets as "text entry active".
		const FString Type = Focused->GetTypeAsString(); // e.g. "SEditableText", "SEditableTextBox", "SMultiLineEditableText"
		return Type.Contains(TEXT("EditableText"));
	}
}

void USystemLinkActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	UE_LOG(LogSystemLink, Warning, TEXT("-{{{ Activated }}}-"));

	// Bind input method change handling
	if (const ULocalPlayer* Lp = GetOwningLocalPlayer())
	{
		if (UCommonInputSubsystem* Cis = UCommonInputSubsystem::Get(Lp))
		{
			Cis->OnInputMethodChangedNative.AddUObject(this, &USystemLinkActivatableWidget::HandleInputMethodChanged);

			HandleInputMethodChanged(Cis->GetCurrentInputType());
		}
	}
}

void USystemLinkActivatableWidget::NativeOnDeactivated()
{
	UE_LOG(LogSystemLink, Warning, TEXT("-{{{ DEActivated }}}-"));
	
	// Unbind input method changes
	if (const ULocalPlayer* Lp = GetOwningLocalPlayer())
	{
		if (UCommonInputSubsystem* Cis = UCommonInputSubsystem::Get(Lp))
		{
			Cis->OnInputMethodChangedNative.RemoveAll(this);
		}
	}

	// ClearTimer();
	
	Super::NativeOnDeactivated();
}

USystemLinkActivatableWidget* USystemLinkActivatableWidget::PushToMenu(const TSubclassOf<USystemLinkActivatableWidget> WidgetClass)
{
	if (!WidgetClass)
	{
		UE_LOG(LogSystemLink, Error, TEXT("PushToMenu: WidgetClass is null, cannot push."));
		return nullptr;
	}

	const ASystemLinkPlayerController* SystemLinkPC = GetSystemLinkPlayerController();
	if (!SystemLinkPC)
	{
		UE_LOG(LogSystemLink, Error, TEXT("PushToMenu: SystemLinkPlayerController is null!"));
		return nullptr;
	}

	USystemLinkActivatableWidget* Result = SystemLinkPC->PushToMenu(WidgetClass);
	if (!Result)
	{
		UE_LOG(LogSystemLink, Warning, TEXT("PushToMenu: Failed to push widget to Menu stack (WidgetClass: %s)."), *WidgetClass->GetName());
	}

	return Result;
}

USystemLinkActivatableWidget* USystemLinkActivatableWidget::PushToHud(const TSubclassOf<USystemLinkActivatableWidget> WidgetClass)
{	
	if (!WidgetClass)
	{
		UE_LOG(LogSystemLink, Error, TEXT("PushToHud: WidgetClass is null, cannot push."));
		return nullptr;
	}

	const ASystemLinkPlayerController* SystemLinkPC = GetSystemLinkPlayerController();
	if (!SystemLinkPC)
	{
		UE_LOG(LogSystemLink, Error, TEXT("PushToHud: SystemLinkPlayerController is null!"));
		return nullptr;
	}

	USystemLinkActivatableWidget* Result = SystemLinkPC->PushToHud(WidgetClass);
	if (!Result)
	{
		UE_LOG(LogSystemLink, Warning, TEXT("PushToHud: Failed to push widget to HUD stack (WidgetClass: %s)."), *WidgetClass->GetName());
	}

	return Result;
}

void USystemLinkActivatableWidget::HandleInputMethodChanged(const ECommonInputType NewType) const
{	
	// Only try to restore focus immediately when switching TO gamepad
	if (bAutoRestoreFocusOnGamepad && NewType == ECommonInputType::Gamepad)
	{
		TryRestoreFocusIfNeeded();
	}
}

ASystemLinkPlayerController* USystemLinkActivatableWidget::GetSystemLinkPlayerController() const
{
	// Get the owning player
	APlayerController* PC = GetOwningPlayer();
	
	// Cast to SystemLinkPlayerController
	ASystemLinkPlayerController* SystemLinkPC = Cast<ASystemLinkPlayerController>(PC);

	if (!SystemLinkPC)
	{
		ensureMsgf(false, TEXT("Failed to cast Owning Player to ASystemLinkPlayerController."));
		return nullptr;
	}

	return SystemLinkPC;
}

void USystemLinkActivatableWidget::TryRestoreFocusIfNeeded() const
{
	// 1) Only act when current input is Gamepad (keep timer alive, be polite on KB/M)
	if (const ULocalPlayer* LP = GetOwningLocalPlayer())
	{
		if (const UCommonInputSubsystem* CIS = UCommonInputSubsystem::Get(LP))
		{
			if (CIS->GetCurrentInputType() != ECommonInputType::Gamepad)
			{
				return; // user is on KB/M; don't steal focus
			}
		}
	}

	// 2) If anything under this widget already has user focus, do nothing
	if (HasAnyUserFocus() || HasFocusedDescendants())
	{
		return;
	}

	// If the user is typing into a text box anywhere, don't steal focus
	if (IsAnyTextEntryActive())
	{
		return;
	}

	// 3) Restore focus to your default target
	FocusDefaultTargetIfPossible();
}

void USystemLinkActivatableWidget::FocusDefaultTargetIfPossible() const
{
	UWidget* DefaultFocusTarget = Super::NativeGetDesiredFocusTarget();
	if (!IsValid(DefaultFocusTarget))
	{
		UE_LOG(LogSystemLink, Warning, TEXT("No valid DefaultFocusTarget!"));
		return;
	}

	// Ensure the widget is visible and enabled
	const bool bVisible = DefaultFocusTarget->IsVisible();
	const bool bEnabled = DefaultFocusTarget->GetIsEnabled();
	if (!bVisible || !bEnabled)
	{
		UE_LOG(LogSystemLink, Warning, TEXT("DefaultFocusTarget is not visible or enabled!"));
		return;
	}

	if (this != GetActiveWidget())
	{
		return; // Don't steal focus if we're not the active widget
	}

	// If the target already has user focus for THIS player, bail
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (DefaultFocusTarget->HasUserFocus(PC))
		{
			UE_LOG(LogSystemLink, Warning, TEXT("DefaultFocusTarget already has user focus!"));
			return;
		}

		// Set user focus (controller-friendly)
		DefaultFocusTarget->SetUserFocus(PC);
	}

	// If it's a CommonButtonBase, ensure it's focusable and set selected
	if (UCommonButtonBase* Button = Cast<UCommonButtonBase>(DefaultFocusTarget))
	{
		Button->SetIsFocusable(true);
		Button->SetIsSelected(true);
	}
}

UCommonActivatableWidget* USystemLinkActivatableWidget::GetActiveWidget() const
{	
	// Cast to SystemLinkPlayerController
	const ASystemLinkPlayerController* SystemLinkPC = GetSystemLinkPlayerController();
	if (!SystemLinkPC)
	{
		UE_LOG(LogSystemLink, Warning, TEXT("Owning Player is not ASystemLinkPlayerController."));
		return nullptr;
	}
	
	// Return the active widget
	return SystemLinkPC->GetActiveWidget();	
}

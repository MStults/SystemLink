#include "SystemLinkActivatableWidget.h"

#include "CommonButtonBase.h"
#include "SystemLinkUiManagerInterface.h"
#include "CommonInputSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Widget.h"
#include "EnhancedInputSubsystems.h" 

void USystemLinkActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	// Bind input method change handling
	if (const ULocalPlayer* Lp = GetOwningLocalPlayer())
	{
		if (UCommonInputSubsystem* Cis = UCommonInputSubsystem::Get(Lp))
		{
			Cis->OnInputMethodChangedNative.AddUObject(this, &USystemLinkActivatableWidget::HandleInputMethodChanged);
		}
	}
	
	StartTimer();

	if (!IMC_UI) return;

	if (ULocalPlayer* LP = GetOwningLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* EIS = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			// Add the UI mapping context with high priority so it wins over gameplay
			EIS->AddMappingContext(IMC_UI, UIInputPriority);
		}
	}
}

void USystemLinkActivatableWidget::NativeOnDeactivated()
{
	// Unbind input method changes
	if (const ULocalPlayer* Lp = GetOwningLocalPlayer())
	{
		if (UCommonInputSubsystem* Cis = UCommonInputSubsystem::Get(Lp))
		{
			Cis->OnInputMethodChangedNative.RemoveAll(this);
		}
	}

	ClearTimer();
	
	Super::NativeOnDeactivated();

	if (!IMC_UI) return;

	if (ULocalPlayer* LP = GetOwningLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* EIS = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			// Remove the UI mapping context when the menu closes
			EIS->RemoveMappingContext(IMC_UI);
		}
	}
}

void USystemLinkActivatableWidget::PushToMenu(const TSubclassOf<UCommonActivatableWidget> WidgetClass) const
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (PC->GetClass()->ImplementsInterface(USystemLinkUiManagerInterface::StaticClass()))
		{
			ISystemLinkUiManagerInterface::Execute_PushToMenu(PC, WidgetClass);
		}
	}
}

void USystemLinkActivatableWidget::PushToHud(const TSubclassOf<UCommonActivatableWidget> WidgetClass) const
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (PC->GetClass()->ImplementsInterface(USystemLinkUiManagerInterface::StaticClass()))
		{
			ISystemLinkUiManagerInterface::Execute_PushToHud(PC, WidgetClass);
		}
	}
}

void USystemLinkActivatableWidget::StartTimer()
{	
	// Make sure this runs in a valid world context
	if (bAutoRestoreFocusOnGamepad && GetWorld() && !GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,               
			this,                     
			&USystemLinkActivatableWidget::TryRestoreFocusIfNeeded,
			0.2f,                      
			true                        
		);
	}
}

void USystemLinkActivatableWidget::ClearTimer()
{
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void USystemLinkActivatableWidget::HandleInputMethodChanged(const ECommonInputType NewType) const
{
	if (bAutoRestoreFocusOnGamepad && NewType == ECommonInputType::Gamepad)
	{
		TryRestoreFocusIfNeeded();
	}
}

void USystemLinkActivatableWidget::TryRestoreFocusIfNeeded() const
{
	// If any of our widgets already has user focus, do nothing.
	if (HasAnyUserFocus() || HasFocusedDescendants())
	{
		return;
	}

	FocusDefaultTargetIfPossible();
}

void USystemLinkActivatableWidget::FocusDefaultTargetIfPossible() const
{
	UWidget* DefaultFocusTarget =  Super::NativeGetDesiredFocusTarget();
	if (!IsValid(DefaultFocusTarget)) // Ensure the widget is valid
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid DefaultFocusTarget!"));
		return;
	}

	// Ensure the widget is visible and enabled
	const bool bVisible = DefaultFocusTarget->IsVisible(); // Checks hierarchical visibility
	const bool bEnabled = DefaultFocusTarget->GetIsEnabled();

	// If not visible or enabled, it’s not interactable
	if (!bVisible || !bEnabled)
	{
		return;
	}

	// Check whether it already has focus (avoid repetitive focus logic)
	if (DefaultFocusTarget->HasKeyboardFocus())
	{
		return;
	}

	// Set user focus
	if (APlayerController* PC = GetOwningPlayer())
	{
		DefaultFocusTarget->SetUserFocus(PC);
	}

	// Properly cast to UCommonButtonBase and set selected state
	if (UCommonButtonBase* Button = Cast<UCommonButtonBase>(DefaultFocusTarget))
	{
		Button->SetIsFocusable(true);
		Button->SetIsSelected(true);
	}

}





#include "SystemLinkSettingsRotator.h"

#include "CommonTextBlock.h"
#include "SystemLink/SystemLink.h"
#include "SystemLink/SystemLinkPlayerController.h"

void USystemLinkSettingsRotator::SetRotatorText(const FText Text) const
{
	if (!RotatorText || Text.IsEmpty()) return;
	RotatorText->SetText(bUseUpperCaseForRotatorText? Text.ToUpper() : Text);
	RotatorText->SetStyle(GetCurrentTextStyleClass());
}

USystemLinkSettingsRotator::USystemLinkSettingsRotator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Initialize focusability before the underlying Slate widget is constructed.
	SetIsFocusable(true);
}

void USystemLinkSettingsRotator::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind arrow buttons for mouse users
	if (LeftArrowButton)
	{
		LeftArrowButton->OnClicked.Clear();
		LeftArrowButton->OnClicked.AddDynamic(this, &USystemLinkSettingsRotator::OnLeftArrowClicked);
	}
	if (RightArrowButton)
	{
		RightArrowButton->OnClicked.Clear();
		RightArrowButton->OnClicked.AddDynamic(this, &USystemLinkSettingsRotator::OnRightArrowClicked);
	}

	// Fallback: Use default options if none are set
	if (Options.Num() == 0)
	{
		Options = {
			NSLOCTEXT("SystemLinkSettings", "NoOptionSet", "No Options Set"),
		};
	}

	PopulateTextLabels(Options);
	
	if (Options.Num() > 0)
	{
		SetSelectedItem(0); // Select the first option by default
	}	
}

void USystemLinkSettingsRotator::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetRotatorText(RotatorDisplayText);
}

void USystemLinkSettingsRotator::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (const TSubclassOf<UCommonTextStyle> TextClass = GetCurrentTextStyleClass(); RotatorText && TextClass)
	{
		RotatorText->SetStyle(TextClass);
	}
}

void USystemLinkSettingsRotator::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (RotatorDescriptionText.IsEmpty()) return;
	
	if (const ASystemLinkPlayerController* OwningPlayer = Cast<ASystemLinkPlayerController>(GetOwningPlayer()))
	{
		OwningPlayer->OnButtonDescriptionTextUpdated.Broadcast(this, RotatorDescriptionText);
	}	
}

void USystemLinkSettingsRotator::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	if (const ASystemLinkPlayerController* OwningPlayer = Cast<ASystemLinkPlayerController>(GetOwningPlayer()))
	{
		OwningPlayer->OnButtonDescriptionTextUpdated.Broadcast(this, FText::GetEmpty());
	}
}

void USystemLinkSettingsRotator::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
}

void USystemLinkSettingsRotator::OnLeftArrowClicked()
{
	ShiftTextLeft();
}

void USystemLinkSettingsRotator::OnRightArrowClicked()
{
	ShiftTextRight();
}

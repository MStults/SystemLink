
#include "SystemLinkSettingsRotator.h"

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
}

void USystemLinkSettingsRotator::OnLeftArrowClicked()
{
	ShiftTextLeft();
}

void USystemLinkSettingsRotator::OnRightArrowClicked()
{
	ShiftTextRight();
}

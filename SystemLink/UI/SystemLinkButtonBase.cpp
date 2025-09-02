// Fill out your copyright notice in the Description page of Project Settings.


#include "SystemLinkButtonBase.h"
#include "CommonTextBlock.h"
#include "SystemLink/SystemLinkPlayerController.h"

void USystemLinkButtonBase::SetButtonText(FText Text) const
{
	if (!ButtonText || Text.IsEmpty()) return;
	ButtonText->SetText(bUseUpperCaseForButtonText? Text.ToUpper() : Text);
	ButtonText->SetStyle(GetCurrentTextStyleClass());
}

void USystemLinkButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText);	
}

void USystemLinkButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (const TSubclassOf<UCommonTextStyle> TextClass = GetCurrentTextStyleClass(); ButtonText && TextClass)
	{
		ButtonText->SetStyle(TextClass);
	}
}

void USystemLinkButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (ButtonDescriptionText.IsEmpty()) return;
	
	if (const ASystemLinkPlayerController* OwningPlayer = Cast<ASystemLinkPlayerController>(GetOwningPlayer()))
	{
		OwningPlayer->OnButtonDescriptionTextUpdated.Broadcast(this, ButtonDescriptionText);
	}	
}

void USystemLinkButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	
	if (const ASystemLinkPlayerController* OwningPlayer = Cast<ASystemLinkPlayerController>(GetOwningPlayer()))
	{
		OwningPlayer->OnButtonDescriptionTextUpdated.Broadcast(this, FText::GetEmpty());
	}
}

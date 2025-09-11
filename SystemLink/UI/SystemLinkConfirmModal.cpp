// Fill out your copyright notice in the Description page of Project Settings.


#include "SystemLinkConfirmModal.h"

#include "CommonTextBlock.h"
#include "SystemLinkButtonBase.h"
#include "Components/DynamicEntryBox.h"
#include "ICommonInputModule.h"
#include "SystemLink/SystemLink.h"
#include "Input/CommonUIInputTypes.h"          // FUIActionTag
#include "CommonInputSettings.h"

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;

	FConfirmScreenButtonInfo OKButtonInfo;
	OKButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
	OKButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Ok"));

	FConfirmScreenButtonInfo HiddenButtonInfo;
	OKButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Hidden;
	OKButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT(""));

	InfoObject->AcceptButtonInfo = OKButtonInfo;	
	InfoObject->BackButtonInfo = HiddenButtonInfo;

	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle,
	const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;

	FConfirmScreenButtonInfo YesButtonInfo;
	YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	YesButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Yes"));

	FConfirmScreenButtonInfo NoButtonInfo;
	NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
	NoButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("No"));

	InfoObject->AcceptButtonInfo = YesButtonInfo;	
	InfoObject->BackButtonInfo = NoButtonInfo;

	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkCancelScreen(const FText& InScreenTitle,
	const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;

	FConfirmScreenButtonInfo OkButtonInfo;
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	OkButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Ok"));

	FConfirmScreenButtonInfo CancelButtonInfo;
	CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
	CancelButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Cancel"));

	InfoObject->AcceptButtonInfo = OkButtonInfo;	
	InfoObject->BackButtonInfo = CancelButtonInfo;

	return InfoObject;
}

void USystemLinkConfirmModal::InitConfirmScreen(const UConfirmScreenInfoObject* InScreenInfoObject,
	TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback)
{
	
	if (!InScreenInfoObject)
	{
		UE_LOG(LogSystemLink, Error, TEXT("InScreenInfoObject is invalid or null."));
		return;
	}

	if (!CommonTextBlock_Title)
	{
		UE_LOG(LogSystemLink, Error, TEXT("CommonTextBlock_Title is invalid or null."));
		return;
	}

	CommonTextBlock_Title->SetText(InScreenInfoObject->ScreenTitle);
	CommonTextBlock_Message->SetText(InScreenInfoObject->ScreenMessage);
	CachedClickedButtonCallback = ClickedButtonCallback;

	//Register Accept Button
	if (!ConfirmBinding.IsValid() && InScreenInfoObject->AcceptButtonInfo.ConfirmScreenButtonType != EConfirmScreenButtonType::Hidden)
	{
		UE_LOG(LogSystemLink, Log, TEXT("Adding  Accept ."));
		
		const FDataTableRowHandle ConfirmRow = ICommonInputModule::GetSettings().GetDefaultClickAction();

		const FSimpleDelegate ConfirmedDelegate = FSimpleDelegate::CreateWeakLambda(this,
			[ this]()
		{
				UE_LOG(LogSystemLink, Display, TEXT("Calling ClickedButton"));
			ClickedButton(EConfirmScreenButtonType::Confirmed);
			DeactivateWidget();
		});

		FBindUIActionArgs ConfirmedArgs = FBindUIActionArgs(ConfirmRow, true, ConfirmedDelegate);
		ConfirmedArgs.OverrideDisplayName = InScreenInfoObject->AcceptButtonInfo.ButtonTextToDisplay; 
		ConfirmBinding = RegisterUIActionBinding(ConfirmedArgs);
	}

	// Register Back Button
	if (!BackBinding.IsValid() && InScreenInfoObject->BackButtonInfo.ConfirmScreenButtonType != EConfirmScreenButtonType::Hidden)
	{
		UE_LOG(LogSystemLink, Log, TEXT("Adding  Back ."));
		
		const FSimpleDelegate CancelledDelegate = FSimpleDelegate::CreateWeakLambda(this,
			[this]()
		{
				UE_LOG(LogSystemLink, Display, TEXT("Calling ClickedButton"));
			ClickedButton(EConfirmScreenButtonType::Closed);
			DeactivateWidget();
		});

		const FDataTableRowHandle BackRow = ICommonInputModule::GetSettings().GetDefaultBackAction();
		FBindUIActionArgs CancelledArgs = FBindUIActionArgs(BackRow, true, CancelledDelegate);
		CancelledArgs.OverrideDisplayName = InScreenInfoObject->BackButtonInfo.ButtonTextToDisplay; 
		BackBinding = RegisterUIActionBinding(CancelledArgs);
	}
}

UWidget* USystemLinkConfirmModal::NativeGetDesiredFocusTarget() const
{
	return nullptr;
}

void USystemLinkConfirmModal::RemoveActionBindings()
{
	if (ConfirmBinding.IsValid())
	{
		UE_LOG(LogSystemLink, Log, TEXT("Removing ConfirmBinding."));
		RemoveActionBinding(ConfirmBinding);
		ConfirmBinding = FUIActionBindingHandle();
	}
	
	if (BackBinding.IsValid())
	{
		UE_LOG(LogSystemLink, Log, TEXT("Removing BackBinding."));
		RemoveActionBinding(BackBinding);
		BackBinding = FUIActionBindingHandle();
	}
}

void USystemLinkConfirmModal::ClickedButton(const EConfirmScreenButtonType ConfirmButtonTypeClicked)
{
	UE_LOG(LogSystemLink, Display, TEXT("IN ClickedButton"));
	if (CachedClickedButtonCallback)
	{
		UE_LOG(LogSystemLink, Display, TEXT("CALLING ConfirmButtonTypeClicked"));
		CachedClickedButtonCallback(ConfirmButtonTypeClicked);
	} else {
		UE_LOG(LogSystemLink, Display, TEXT("No Callback"));
	}
}

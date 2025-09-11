// Fill out your copyright notice in the Description page of Project Settings.


#include "SystemLinkOptionsScreen.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"

void USystemLinkOptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				ResetAction,
				true,
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)
			)
		);
	}

	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this,&ThisClass::OnBackBoundActionTriggered)
		)
	);
}

void USystemLinkOptionsScreen::OnResetBoundActionTriggered()
{
	UE_LOG(LogTemp, Warning, TEXT("Reset Bound Action Triggered"));
}

void USystemLinkOptionsScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

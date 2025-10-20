// Fill out your copyright notice in the Description page of Project Settings.

#include "SystemLinkOptionsScreen.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "Data/SystemLinkListItemCollection.h"
#include "Data/SystemLinkOptionsItemRegistry.h"
#include "SystemLinkButtonBase.h"
#include "SystemLinkTabListWidgetBase.h"
#include "SystemLink/SystemLink.h"

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

	TabListWidget_OptionsTabs->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnOptionsTabSelected);
}

void USystemLinkOptionsScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (USystemLinkListItemCollection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollections())
	{
		if (!TabCollection)
		{
			continue;
		}

		const FName TabID = TabCollection->GetDataID();

		if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID) != nullptr)
		{  
			continue; 
		}

		TabListWidget_OptionsTabs->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
	}
}

USystemLinkOptionsItemRegistry* USystemLinkOptionsScreen::GetOrCreateDataRegistry()
{
	if (!CreatedOwningDataRegistry)
	{
		CreatedOwningDataRegistry = NewObject<USystemLinkOptionsItemRegistry>();
		CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}

	checkf(CreatedOwningDataRegistry,TEXT("Data registry for options screen is not valid"));

	return CreatedOwningDataRegistry;
}

void USystemLinkOptionsScreen::OnResetBoundActionTriggered()
{
	UE_LOG(LogSystemLink, Warning, TEXT("Reset Bound Action Triggered"));
}

void USystemLinkOptionsScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

void USystemLinkOptionsScreen::OnOptionsTabSelected(FName TabId)
{
	UE_LOG(LogSystemLink, Warning, TEXT("Tab Selected Action Triggered"));
}

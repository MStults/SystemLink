// Fill out your copyright notice in the Description page of Project Settings.


#include "SystemLinkOptionsItemRegistry.h"

#include "SystemLinkListItemCollection.h"

void USystemLinkOptionsItemRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab();
}

void USystemLinkOptionsItemRegistry::InitGameplayCollectionTab()
{
	USystemLinkListItemCollection* GameplayTabCollection = NewObject<USystemLinkListItemCollection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString(TEXT("Gameplay")));

	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

void USystemLinkOptionsItemRegistry::InitAudioCollectionTab()
{
	USystemLinkListItemCollection* AudioTabCollection = NewObject<USystemLinkListItemCollection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString(TEXT("Audio")));

	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void USystemLinkOptionsItemRegistry::InitVideoCollectionTab()
{
	USystemLinkListItemCollection* VideoTabCollection = NewObject<USystemLinkListItemCollection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString(TEXT("Video")));

	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void USystemLinkOptionsItemRegistry::InitControlCollectionTab()
{
	USystemLinkListItemCollection* ControlTabCollection = NewObject<USystemLinkListItemCollection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString(TEXT("Control")));

	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}

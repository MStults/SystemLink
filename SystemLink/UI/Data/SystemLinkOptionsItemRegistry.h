// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SystemLinkOptionsItemRegistry.generated.h"

class USystemLinkListItemCollection;
/**
 * 
 */
UCLASS()
class SYSTEMLINK_API USystemLinkOptionsItemRegistry : public UObject
{
	GENERATED_BODY()

public:
	//Gets called by the options' screen right after the object of type UOptionsDataRegistry is created
	void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);

	const TArray<USystemLinkListItemCollection*>& GetRegisteredOptionsTabCollections() const
	{
		return RegisteredOptionsTabCollections;
	}

private:
	void InitGameplayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControlCollectionTab();

	UPROPERTY(Transient)
	TArray<USystemLinkListItemCollection*> RegisteredOptionsTabCollections;
};

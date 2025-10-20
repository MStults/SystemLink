// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SystemLinkListItemBase.h"
#include "SystemLinkListItemCollection.generated.h"

/**
 * 
 */
UCLASS()
class SYSTEMLINK_API USystemLinkListItemCollection : public USystemLinkListItemBase
{
	GENERATED_BODY()

public:
	void AddChild(USystemLinkListItemBase* InChild);
	
	
	//~ Begin USystemLinkListItemBase Interface
	virtual TArray<USystemLinkListItemBase*> GetChildren() const override;
	virtual bool HasChildren() const override;
	//~ End USystemLinkListItemBase Interface

private:
	UPROPERTY(Transient)
	TArray<USystemLinkListItemBase*> ChildrenArray;
};

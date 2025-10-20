// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SystemLinkListItemBase.generated.h"

#define LIST_ITEM_ACCESSOR(DataType,PropertyName) \
FORCEINLINE DataType Get##PropertyName() const { return PropertyName;} \
void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName;}

/**
 * 
 */
UCLASS()
class SYSTEMLINK_API USystemLinkListItemBase : public UObject
{
	GENERATED_BODY()
public:
	LIST_ITEM_ACCESSOR(FName,DataID)
	LIST_ITEM_ACCESSOR(FText,DataDisplayName)
	LIST_ITEM_ACCESSOR(FText,DescriptionRichText)
	LIST_ITEM_ACCESSOR(FText,DisabledRichText)
	LIST_ITEM_ACCESSOR(TSoftObjectPtr<UTexture2D>,SoftDescriptionImage)
	LIST_ITEM_ACCESSOR(USystemLinkListItemBase*,ParentData)

	void InitDataObject();

	//Empty in the base class. Child class ListDataObject_Collection should override it. The function should return all the child data a tab has
	virtual TArray<USystemLinkListItemBase*> GetChildren() const { return TArray<USystemLinkListItemBase*>();}
	virtual bool HasChildren() const { return false; }

protected:
	//Empty in base class. The child classes should override it to handle the initialization needed accordingly
	virtual void OnDataObjectInitialized();
	
private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;  
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;

	UPROPERTY(Transient)
	USystemLinkListItemBase* ParentData;
};
 
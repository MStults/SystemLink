// Fill out your copyright notice in the Description page of Project Settings.


#include "SystemLinkListItemCollection.h"

void USystemLinkListItemCollection::AddChild(USystemLinkListItemBase* InChild)
{
	//Notify the child list data to init itself
	InChild->InitDataObject();

	//Set the child list data's parent to this
	InChild->SetParentData(this);
	
	ChildrenArray.Add(InChild);
}

TArray<USystemLinkListItemBase*> USystemLinkListItemCollection::GetChildren() const
{
	return ChildrenArray;
}

bool USystemLinkListItemCollection::HasChildren() const
{
	return !ChildrenArray.IsEmpty();
}

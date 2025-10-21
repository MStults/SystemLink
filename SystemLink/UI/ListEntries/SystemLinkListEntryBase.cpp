// Fill out your copyright notice in the Description page of Project Settings.


#include "SystemLinkListEntryBase.h"

void USystemLinkListEntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

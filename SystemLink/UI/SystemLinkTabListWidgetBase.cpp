// Fill out your copyright notice in the Description page of Project Settings.

#include "SystemLinkTabListWidgetBase.h"
#include "Editor/WidgetCompilerLog.h"
#include "SystemLinkButtonBase.h"

void USystemLinkTabListWidgetBase::RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName)
{
	RegisterTab(InTabID,TabButtonEntryWidgetClass,nullptr);

	if (const USystemLinkButtonBase* FoundButton = Cast<USystemLinkButtonBase>(GetTabButtonBaseByID(InTabID)))
	{
		FoundButton->SetButtonText(InTabDisplayName);
	}
}

#if WITH_EDITOR	
void USystemLinkTabListWidgetBase::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(
			TEXT("The variable TabButtonEntryWidgetClass has no valid entry specified. ") +
			GetClass()->GetName() + 
			TEXT(" needs a valid entry widget class to function properly")
		));
	}
}
#endif
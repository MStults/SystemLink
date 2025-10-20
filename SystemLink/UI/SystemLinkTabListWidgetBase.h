// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "SystemLinkTabListWidgetBase.generated.h"

class USystemLinkButtonBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class SYSTEMLINK_API USystemLinkTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()
public:
	void RequestRegisterTab(const FName& InTabID,const FText& InTabDisplayName);
private:
	
#if WITH_EDITOR	
	/**
	 Validates the widget's compiled defaults to ensure required configuration is set.
	 */
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SystemLink", meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "10"))
	int32 DebugEditorPreviewTabCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SystemLink", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USystemLinkButtonBase> TabButtonEntryWidgetClass;
};

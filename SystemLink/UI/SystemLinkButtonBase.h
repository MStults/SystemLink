// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "SystemLinkButtonBase.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class SYSTEMLINK_API USystemLinkButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText Text) const;

	private:
	virtual void NativePreConstruct() override;
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* ButtonText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SystemLink", meta=(AllowPrivateAccess="true"))
	FText ButtonDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SystemLink", meta=(AllowPrivateAccess="true"))
	bool bUseUpperCaseForButtonText = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SystemLink", meta=(AllowPrivateAccess="true"))
	FText ButtonDescriptionText;
	
	public:
};

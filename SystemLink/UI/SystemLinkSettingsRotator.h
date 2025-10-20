#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CommonRotator.h"
#include "SystemLinkSettingsRotator.generated.h"

class UButton;
class UCommonTextBlock;

/**
 * 
 */
UCLASS()
class SYSTEMLINK_API USystemLinkSettingsRotator : public UCommonRotator
{
	GENERATED_BODY()
public:
	USystemLinkSettingsRotator(const FObjectInitializer& ObjectInitializer);
	
	// Helpers so your menu code is clean
	UFUNCTION(BlueprintCallable, Category="SystemLink")
	void SetOptions(const TArray<FText>& InOptions) { PopulateTextLabels(InOptions); }

	UFUNCTION(BlueprintCallable, Category="SystemLink")
	void SetSelectedIndex_Safe(int32 Index) { SetSelectedItem(Index); } // wraps base

	
	UFUNCTION(BlueprintCallable)
	void SetRotatorText(FText Text) const;
	
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;

	// Disable click-to-rotate on the main button; arrows handle mouse interaction.
	virtual void NativeOnClicked() override {}

	// Arrow handlers
	UFUNCTION() void OnLeftArrowClicked();
	UFUNCTION() void OnRightArrowClicked();
	
	// REQUIRED: your BP child must have these exact names
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> LeftArrowButton = nullptr;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> RightArrowButton = nullptr;
		
	UPROPERTY(meta=(BindWidgetOptional))	
	UCommonTextBlock* RotatorText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SystemLink")
	FText RotatorDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SystemLink")
	bool bUseUpperCaseForRotatorText = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SystemLink")
	FText RotatorDescriptionText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SystemLink")
	TArray<FText> Options;
};
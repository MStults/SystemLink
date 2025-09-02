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
	UFUNCTION(BlueprintCallable, Category="Rotator")
	void SetOptions(const TArray<FText>& InOptions) { PopulateTextLabels(InOptions); }

	UFUNCTION(BlueprintCallable, Category="Rotator")
	void SetSelectedIndex_Safe(int32 Index) { SetSelectedItem(Index); } // wraps base

protected:
	virtual void NativeConstruct() override;

	// Disable click-to-rotate on the main button; arrows handle mouse interaction.
	virtual void NativeOnClicked() override {}

	// Arrow handlers
	UFUNCTION() void OnLeftArrowClicked();
	UFUNCTION() void OnRightArrowClicked();

protected:
	// REQUIRED: your BP child must have these exact names
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> LeftArrowButton = nullptr;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> RightArrowButton = nullptr;
};
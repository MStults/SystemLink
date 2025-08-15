// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SystemLinkActivatableWidget.generated.h"

/**
 * A widget specifically designed to integrate with the SystemLink UI architecture.
 * Provides functionality to manage activatable widgets within the UI Manager.
 */
UCLASS()
class SYSTEMLINK_API USystemLinkActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SystemLink|Focus")
	bool bAutoRestoreFocusOnGamepad = true;

	// Assign your IMC asset in the BP derived from this class
	UPROPERTY(EditDefaultsOnly, Category="Input|UI")
	TObjectPtr<UInputMappingContext> IMC_UI;

	// Higher priority beats gameplay contexts (tune as needed)
	UPROPERTY(EditDefaultsOnly, Category="Input|UI")
	int32 UIInputPriority = 100;
	
	/** Pushes the given widget class to the Menu stack via the UI Manager interface */
	UFUNCTION(BlueprintCallable, Category = "SystemLink")
	void PushToMenu(TSubclassOf<UCommonActivatableWidget> WidgetClass) const;
	
	/** Pushes the given widget class to the HUD stack via the UI Manager interface */
	UFUNCTION(BlueprintCallable, Category = "SystemLink")
	void PushToHud(TSubclassOf<UCommonActivatableWidget> WidgetClass) const;

protected:
	/** Handle activation logic (including input binding) */
	virtual void NativeOnActivated() override;

	/** Handle deactivation logic (including input unbinding) */
	virtual void NativeOnDeactivated() override;

private:
	UFUNCTION()
	void HandleInputMethodChanged(ECommonInputType NewType) const;

	UFUNCTION()
	void TryRestoreFocusIfNeeded() const;

	UFUNCTION()
	void FocusDefaultTargetIfPossible() const;

	FTimerHandle TimerHandle;
	void StartTimer();
	void ClearTimer();
};

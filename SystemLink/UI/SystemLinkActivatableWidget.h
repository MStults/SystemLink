// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonInputTypeEnum.h"
#include "SystemLinkActivatableWidget.generated.h"

class ASystemLinkPlayerController;
/**
 * A widget specifically designed to integrate with the SystemLink UI architecture.
 * Provides functionality to manage activatable widgets within the UI Manager.
 */
UCLASS(Blueprintable, BlueprintType, meta=(DisableNaiveTick))
class SYSTEMLINK_API USystemLinkActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SystemLink")
	bool bAutoRestoreFocusOnGamepad = true;
 
	/** Pushes the given widget class to the Menu stack via the UI Manager interface */
	UFUNCTION(BlueprintCallable, Category = "SystemLink")
	USystemLinkActivatableWidget* PushToMenu(TSubclassOf<USystemLinkActivatableWidget> WidgetClass);
	
	/** Pushes the given widget class to the HUD stack via the UI Manager interface */
	UFUNCTION(BlueprintCallable, Category = "SystemLink")
	USystemLinkActivatableWidget* PushToHud(TSubclassOf<USystemLinkActivatableWidget> WidgetClass);

	UFUNCTION(BlueprintImplementableEvent, Category = "SystemLink")
	void OnLeftFaceButtonPressed();

	UFUNCTION(BlueprintCallable, Category = "SystemLink")
	void TriggerLeftFaceButtonPressed()
	{
		OnLeftFaceButtonPressed(); 
	}

	UFUNCTION(BlueprintCallable, Category = "SystemLink")
	ASystemLinkPlayerController* GetSystemLinkPlayerController() const;

	UFUNCTION()
	void TryRestoreFocusIfNeeded() const;

protected:
	/** Handle activation logic (including input binding) */
	virtual void NativeOnActivated() override;

	/** Handle deactivation logic (including input unbinding) */
	virtual void NativeOnDeactivated() override;

private:
	UFUNCTION()
	void HandleInputMethodChanged(ECommonInputType NewType) const;	

	UFUNCTION()
	void FocusDefaultTargetIfPossible() const;

	UFUNCTION()
	UCommonActivatableWidget* GetActiveWidget() const;	
};

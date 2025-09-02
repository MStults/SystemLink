// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommonUI/Public/Widgets/CommonActivatableWidgetContainer.h"
#include "SystemLinkUiRootContainer.generated.h"

/**
 * @class USystemLinkUiRootContainer
 * @brief A user widget container that manages and provides access to HUD and Menu widget stacks.
 *
 * This class serves as a container for widget stacks, allowing blueprint access to the HUD and Menu widget stacks.
 * It is primarily used to manage and interact with UI elements for HUD and Menu functionalities.
 */
UCLASS()
class SYSTEMLINK_API USystemLinkUiRootContainer : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SystemLink")
	UCommonActivatableWidgetStack* GetHudStack() const { return HudStack; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SystemLink")
	UCommonActivatableWidgetStack* GetMenuStack() const { return MenuStack; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SystemLink")
	UCommonActivatableWidgetStack* GetModalStack() const { return ModalStack; }

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> HudStack;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> MenuStack;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> ModalStack;
};

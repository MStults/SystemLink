// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CommonActivatableWidget.h"
#include "SystemLinkUiManagerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USystemLinkUiManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @class ISystemLinkUiManagerInterface
 * @brief Interface for managing user interface interactions.
 *
 * The IIuiManagerInterface defines the core functionality for handling
 * UI-related operations such as displaying a pause menu or manipulating
 * UI widgets on the stack. Classes that implement this interface are
 * responsible for providing concrete behavior for these UI actions.
 */
class SYSTEMLINK_API ISystemLinkUiManagerInterface
{
	GENERATED_BODY()	

public:

	// Show a widget and handle focus logic (preferred)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SystemLink")
	UCommonActivatableWidget* ShowMenu(TSubclassOf<class UCommonActivatableWidget> WidgetClass);

	// Push a widget to the Menu UI stack
	UFUNCTION(BlueprintNativeEvent, Category = "SystemLink", meta = (BlueprintInternalUseOnly = "true"))
	UCommonActivatableWidget* PushToMenu(TSubclassOf<class UCommonActivatableWidget> WidgetClass);

	// Push a widget to the HUD UI stack
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SystemLink")
	UCommonActivatableWidget* PushToHud(TSubclassOf<class UCommonActivatableWidget> WidgetClass);

	// Show a widget and handle focus logic (preferred)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SystemLink")
	UCommonActivatableWidget* ShowModal(TSubclassOf<class UCommonActivatableWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SystemLink")
	UCommonActivatableWidget* GetActiveWidget() const;
};


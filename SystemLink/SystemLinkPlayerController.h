// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CommonActivatableWidget.h"
#include "CommonUI/Public/Widgets/CommonActivatableWidgetContainer.h"
#include "UI/SystemLinkUiManagerInterface.h"
#include "UI/SystemLinkUiRootContainer.h"
#include "SystemLinkPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
		FOnButtonDescriptionTextUpdatedDelegate,
		USystemLinkButtonBase*,
		BroadcastingButton,
		FText,
		DescriptionText
	);

/**
 * @class ASystemLinkPlayerController
 * @brief A specialized player controller class for managing UI interactions within the System Link plugin.
 *
 * This class extends APlayerController and implements the IIuiManagerInterface to provide functionality
 * for handling the user interface (UI) stack for both the menu and Heads-Up Display (HUD). It manages
 * the instantiation, addition, and activation of UI widgets for menus and HUD elements.
 */
UCLASS()
class SYSTEMLINK_API ASystemLinkPlayerController : public APlayerController, public ISystemLinkUiManagerInterface
{
	GENERATED_BODY()

	static UCommonActivatableWidget* PushToStack_Internal(const TSubclassOf<class UCommonActivatableWidget>& WidgetClass, UCommonActivatableWidgetStack* Stack);
	UCommonActivatableWidget* ShowWidget_Internal(const TSubclassOf<class UCommonActivatableWidget>& WidgetClass, UCommonActivatableWidgetStack* Stack);

public: 
	virtual void BeginPlay() override;

	// Interface overrides
	virtual UCommonActivatableWidget*  PushToMenu_Implementation(TSubclassOf<UCommonActivatableWidget> WidgetClass) override;
	virtual UCommonActivatableWidget*  PushToHud_Implementation(TSubclassOf<UCommonActivatableWidget> WidgetClass) override;
	virtual UCommonActivatableWidget*  ShowMenu_Implementation(TSubclassOf<UCommonActivatableWidget> WidgetClass) override;
	virtual UCommonActivatableWidget*  ShowModal_Implementation(TSubclassOf<UCommonActivatableWidget> WidgetClass) override;

	virtual UCommonActivatableWidget*  GetActiveWidget_Implementation() const override;

	/**
	 * @brief Event triggered when the UI root container is ready for interaction.
	 *
	 * This BlueprintImplementableEvent is called after the UI root widget, which encapsulates
	 * the HUD and menu stacks, is successfully created and added to the viewport.
	 * It allows developers to customize behavior and operations that need to occur
	 * once the UI setup is initialized and ready to use.
	 *
	 * @note This is expected to be implemented in a Blueprint to define specific
	 *       custom logic upon UI initialization.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnUIRootReady(USystemLinkUiRootContainer* RootContainer);

	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdatedDelegate OnButtonDescriptionTextUpdated;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class USystemLinkUiRootContainer> UIRootClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCommonActivatableWidgetStack> MenuStack;
 
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCommonActivatableWidgetStack> HudStack;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCommonActivatableWidgetStack> ModalStack;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USystemLinkUiRootContainer> UIRoot;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void TryRestoreFocusIfNeeded() const;
	
	FTimerHandle SystemLinkActivatableWidgetTimerHandle;
	void StartSystemLinkActivatableWidgetTimer();
	void ClearSystemLinkActivatableWidgetTimer();
};

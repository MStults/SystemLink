// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CommonUI/Public/Widgets/CommonActivatableWidgetContainer.h"
#include "UI/SystemLinkConfirmModal.h"
#include "UI/SystemLinkUiRootContainer.h"
#include "UI/SystemLinkUITypes.h"
#include "SystemLinkPlayerController.generated.h"

class USystemLinkConfirmAsyncAction;
class USystemLinkActivatableWidget;
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
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class SYSTEMLINK_API ASystemLinkPlayerController : public APlayerController
{
	GENERATED_BODY()

	static USystemLinkActivatableWidget* PushToStack_Internal(const TSubclassOf<class USystemLinkActivatableWidget>& WidgetClass, UCommonActivatableWidgetStack* Stack);
	USystemLinkActivatableWidget* ShowWidget_Internal(const TSubclassOf<class USystemLinkActivatableWidget>& WidgetClass, UCommonActivatableWidgetStack* Stack);

public: 
	virtual void OnPossess(APawn* PossessPawn) override;

	//UFUNCTION(BlueprintCallable, Category = "SystemLink")
	USystemLinkActivatableWidget* PushToMenu(const TSubclassOf<USystemLinkActivatableWidget> WidgetClass) const;

	//UFUNCTION(BlueprintCallable, Category = "SystemLink")
	USystemLinkActivatableWidget* PushToHud(const TSubclassOf<USystemLinkActivatableWidget> WidgetClass) const;

	UFUNCTION(BlueprintCallable, Category = "SystemLink")
	USystemLinkActivatableWidget* ShowMenu(const TSubclassOf<USystemLinkActivatableWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, Category = "SystemLink")
	USystemLinkActivatableWidget* GetActiveWidget() const; 

	void ShowModal(EConfirmScreenType ConfirmScreenType, const FText& InScreenTitle,const FText& InScreenMsg, const TFunction<void(EConfirmScreenButtonType)>& ButtonClickedCallback) const;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SystemLink")
	TSubclassOf<class USystemLinkUiRootContainer> UIRootClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SystemLink")
	TSubclassOf<USystemLinkConfirmModal> YesNoModalClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SystemLink")
	TSubclassOf<USystemLinkConfirmModal> OkCancelModalClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SystemLink")
	TSubclassOf<USystemLinkConfirmModal> OkModalClass;
	
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

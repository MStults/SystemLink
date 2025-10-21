// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SystemLinkActivatableWidget.h"
#include "SystemLinkOptionsScreen.generated.h"

class USystemLinkTabListWidgetBase;
class USystemLinkOptionsItemRegistry;
class USystemLinkListItemBase;
class USystemLinkListView;

/**
 * @class USystemLinkOptionsScreen
 * @brief A system link options screen widget that extends USystemLinkActivatableWidget.
 *
 * The USystemLinkOptionsScreen provides functionality for initializing and handling
 * UI actions specific to system link options, such as reset and back actions. It overrides
 * the initialization behavior to bind specific actions to their respective callbacks.
 */
UCLASS(Blueprintable, BlueprintType, meta=(DisableNaiveTick))
class SYSTEMLINK_API USystemLinkOptionsScreen : public USystemLinkActivatableWidget
{
	GENERATED_BODY()

protected:
	//~ Begin UUserWidget  Interface
	virtual void NativeOnInitialized() override;	
	//~ End UUserWidget  Interface
	
private:
	//~ Begin UCommonActivatableWidget Interface	
	virtual void NativeOnActivated() override;
	//~ End UCommonActivatableWidget Interface

	USystemLinkOptionsItemRegistry* GetOrCreateDataRegistry();
	
	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	//***** Bound Widgets ***** //
	UPROPERTY(meta = (BindWidget))
	USystemLinkTabListWidgetBase* TabListWidget_OptionsTabs;

	UPROPERTY(meta = (BindWidget))
	USystemLinkListView* ListView_OptionsList;
	//***** Bound Widgets ***** //
	
	//Handle the creation of data in the option screen. Direct access to this variable is forbidden
	UPROPERTY(Transient)
	USystemLinkOptionsItemRegistry* CreatedOwningDataRegistry;

	UPROPERTY(EditDefaultsOnly, Category = "SystemLink", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;
	int OnTabSelected;

	UFUNCTION()
	void OnOptionsTabSelected(FName TabId);
};
   
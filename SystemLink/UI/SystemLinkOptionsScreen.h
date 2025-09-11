// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SystemLinkActivatableWidget.h"
#include "SystemLinkOptionsScreen.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, meta=(DisableNaiveTick))
class SYSTEMLINK_API USystemLinkOptionsScreen : public USystemLinkActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	
private:	
	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	UPROPERTY(EditDefaultsOnly, Category = "SystemLink", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;
};

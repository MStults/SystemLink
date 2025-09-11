// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SystemLinkActivatableWidget.h"
#include "SystemLinkUITypes.h"
#include "Input/CommonBoundActionBar.h"
#include "SystemLinkConfirmModal.generated.h"

class UDynamicEntryBox;
class UCommonTextBlock;

USTRUCT(BlueprintType)
struct FConfirmScreenButtonInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EConfirmScreenButtonType ConfirmScreenButtonType = EConfirmScreenButtonType::Unknown;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText ButtonTextToDisplay;
};

UCLASS()
class SYSTEMLINK_API UConfirmScreenInfoObject : public UObject
{
	GENERATED_BODY()	

public:
	static UConfirmScreenInfoObject* CreateOKScreen(const FText& InScreenTitle,const FText& InScreenMsg);
	static UConfirmScreenInfoObject* CreateYesNoScreen(const FText& InScreenTitle,const FText& InScreenMsg);
	static UConfirmScreenInfoObject* CreateOkCancelScreen(const FText& InScreenTitle,const FText& InScreenMsg);

	UPROPERTY(Transient)
	FText ScreenTitle;
	
	UPROPERTY(Transient)
	FText ScreenMessage;

	UPROPERTY(Transient)
	FConfirmScreenButtonInfo AcceptButtonInfo;

	UPROPERTY(Transient)
	FConfirmScreenButtonInfo BackButtonInfo;
};

UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class SYSTEMLINK_API USystemLinkConfirmModal : public USystemLinkActivatableWidget //UCommonActivatableWidget
{
	GENERATED_BODY()

	TFunction<void(EConfirmScreenButtonType)> CachedClickedButtonCallback = nullptr;
	
public:
	// Gets called outside the class when this widget is constructed, and before it's pushed to the modal stack
	void InitConfirmScreen(const UConfirmScreenInfoObject* InScreenInfoObject,TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback);

protected:
	/** Override to provide the desired widget to focus when this becomes active */
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Message;

	// This is not used directly in this class, but an actionbar is required when Registering UI Action Binding.
	UPROPERTY(meta=(BindWidget)) 
	UCommonBoundActionBar* ActionBar = nullptr;

	/** Handles for removing action bindings on deactivation. */
	FUIActionBindingHandle ConfirmBinding;
	FUIActionBindingHandle BackBinding;
	void RemoveActionBindings();
	void ClickedButton(EConfirmScreenButtonType ConfirmButtonTypeClicked);
};

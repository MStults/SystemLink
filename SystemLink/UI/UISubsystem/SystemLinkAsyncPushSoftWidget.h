// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "Kismet/BlueprintAsyncActionBase.h"
// #include "SystemLinkAsyncPushSoftWidget.generated.h"
//
// enum class ESystemLinkWidgetStackType : uint8;
// class USystemLinkActivatableWidget;
//
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate,USystemLinkActivatableWidget*,PushedWidget);
// /**
//  * 
//  */
// UCLASS()
// class SYSTEMLINK_API USystemLinkAsyncPushSoftWidget : public UBlueprintAsyncActionBase
// {
// 	GENERATED_BODY()
//
// public:
// 	//~ Begin UBlueprintAsyncActionBase Interface
// 	virtual void Activate() override;
// 	//~ End UBlueprintAsyncActionBase Interface
// 	
// 	// //~ Begin UObject Interface
// 	// virtual void FinishDestroy() override;
// 	// //~ End UObject Interface
//
// 	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Push Soft Widget To Widget Stack"))
// 	static USystemLinkAsyncPushSoftWidget* PushSoftWidgetToStackAsync(
// 		const UObject* WorldContextObject,
// 		APlayerController* OwningPlayerController,
// 		TSoftClassPtr<USystemLinkActivatableWidget> InSoftWidgetClass,
// 		ESystemLinkWidgetStackType StackType,
// 		bool bFocusOnNewlyPushedWidget = true
// 	);
//
// 	UPROPERTY(BlueprintAssignable)
// 	FOnPushSoftWidgetDelegate OnWidgetCreatedBeforePush;
//
// 	UPROPERTY(BlueprintAssignable)
// 	FOnPushSoftWidgetDelegate AfterPush;
//
// private:
// 	TWeakObjectPtr<UWorld> CachedOwningWorld;
// 	TWeakObjectPtr<APlayerController> CachedOwningPC;
// 	TSoftClassPtr<USystemLinkActivatableWidget> CachedSoftWidgetClass;
// 	ESystemLinkWidgetStackType CachedStackType;
// 	bool bCachedFocusOnNewlyPushedWidget = false;
// };

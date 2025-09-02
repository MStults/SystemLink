// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "SystemLinkUiRootContainer.h"
// #include "Subsystems/GameInstanceSubsystem.h"
// #include "SystemLinkUISubsystem.generated.h"
//
// class USystemLinkUiRootContainer;
// class USystemLinkActivatableWidget;
//
// enum class EAsyncPushWidgetState : uint8
// {
// 	OnCreatedBeforePush,
// 	AfterPush
// };
// /**
//  * 
//  */
// UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
// class SYSTEMLINK_API USystemLinkUISubsystem : public UGameInstanceSubsystem
// {
// 	GENERATED_BODY()
//
// public:
// 	static USystemLinkUISubsystem* Get(const UObject* WorldContextObject);
//
// 	//~ Begin USubsystem Interface	
// 	virtual bool ShouldCreateSubsystem(UObject* Outer) const override; //Used to ensure there is only one (Singleton)
// 	//~ End USubsystem Interface
//
// 	UFUNCTION(BlueprintCallable, Category = "SystemLink")
// 	void RegisterRootContainer(USystemLinkUiRootContainer* InRootContainer);
//
// 	void PushSoftWidgetToStackAsync(
// 		ESystemLinkWidgetStackType StackType,
// 		TSoftClassPtr<USystemLinkActivatableWidget> InSoftWidgetClass,
// 		TFunction<void(EAsyncPushWidgetState, USystemLinkActivatableWidget*)> AsyncPushStateCallback) const;
//
// private:
// 	
// 	USystemLinkUiRootContainer* RootContainer;
// 	
// };

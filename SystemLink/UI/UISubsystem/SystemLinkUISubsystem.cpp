// // Fill out your copyright notice in the Description page of Project Settings.
//
// #include "SystemLinkUISubsystem.h"
//
// #include "SystemLinkActivatableWidget.h"
// #include "Engine/AssetManager.h"
// #include "Engine/StreamableManager.h"
//
// USystemLinkUISubsystem* USystemLinkUISubsystem::Get(const UObject* WorldContextObject)
// {
// 	if (GEngine)
// 	{
// 		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 		
// 		return UGameInstance::GetSubsystem<USystemLinkUISubsystem>(World->GetGameInstance());		
// 	}
// 	return nullptr;
// }
//
// /**
//  * Determines whether the subsystem should be created for the given outer object.
//  * Ensures there is only one instance of the subsystem (Singleton pattern)
//  * and skips creation if the instance is meant for a dedicated server.
//  *
//  * @param Outer The outer UObject for which the subsystem creation is being considered.
//  *              Expected to be of type UGameInstance.
//  * @return true if the subsystem should be created, false otherwise.
//  */
// bool USystemLinkUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
// {
// 	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
// 	{
// 		TArray<UClass*> FoundClasses;
// 		GetDerivedClasses(GetClass(),FoundClasses);
//
// 		return FoundClasses.IsEmpty();
// 	}
// 	return false;
// }
//
// void USystemLinkUISubsystem::RegisterRootContainer(USystemLinkUiRootContainer* InRootContainer)
// {
// 	check(InRootContainer);
// 	RootContainer = InRootContainer;
// }
//
// void USystemLinkUISubsystem::PushSoftWidgetToStackAsync(
// 	ESystemLinkWidgetStackType StackType,
// 	TSoftClassPtr<USystemLinkActivatableWidget> InSoftWidgetClass,
// 	TFunction<void(EAsyncPushWidgetState, USystemLinkActivatableWidget*)> AsyncPushStateCallback) const
// {
// 	check(!InSoftWidgetClass.IsNull());
//
// 	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
// 		InSoftWidgetClass.ToSoftObjectPath(),
// 		FStreamableDelegate::CreateLambda(
// 			[InSoftWidgetClass,this, StackType, AsyncPushStateCallback]()
// 			{
// 				UClass* LoadedWidgetClass = InSoftWidgetClass.Get();
// 				
// 				check(LoadedWidgetClass);
//
// 				const auto FoundWidgetStack = RootContainer->GetWidgetStack(StackType);
// 				
// 				USystemLinkActivatableWidget* CreatedWidget = FoundWidgetStack->AddWidget<USystemLinkActivatableWidget>(
// 					LoadedWidgetClass,
// 					[AsyncPushStateCallback](USystemLinkActivatableWidget& CreatedWidgetInstance)
// 					{
// 						AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush,&CreatedWidgetInstance);
// 					}
// 				);
// 				
// 				AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush,CreatedWidget);
// 			}
// 		)
// 	);
// }

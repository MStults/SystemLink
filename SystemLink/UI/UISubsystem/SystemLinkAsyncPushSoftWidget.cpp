// // Fill out your copyright notice in the Description page of Project Settings.
//
//
// #include "SystemLinkAsyncPushSoftWidget.h"
//
// #include "SystemLinkActivatableWidget.h"
// #include "SystemLinkUISubsystem.h"
//
// void USystemLinkAsyncPushSoftWidget::Activate()
// {
// 	Super::Activate();
//
// 	const USystemLinkUISubsystem* SystemLinkUISubsystem = USystemLinkUISubsystem::Get(CachedOwningWorld.Get());
//
// 	SystemLinkUISubsystem->PushSoftWidgetToStackAsync(
// 		CachedStackType,
// 		CachedSoftWidgetClass,
// 		[this](const EAsyncPushWidgetState State, USystemLinkActivatableWidget* PushedWidget)
// 		{
// 			if (State == EAsyncPushWidgetState::OnCreatedBeforePush)
// 			{
// 				PushedWidget->SetOwningPlayer(CachedOwningPC.Get());
//
// 				OnWidgetCreatedBeforePush.Broadcast(PushedWidget);
// 			}
// 			else if (State == EAsyncPushWidgetState::AfterPush)
// 			{
// 				AfterPush.Broadcast(PushedWidget);
//
// 				if (bCachedFocusOnNewlyPushedWidget)
// 				{
// 					if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
// 					{
// 						WidgetToFocus->SetFocus();
// 					}
// 				}
//
// 				SetReadyToDestroy();
// 			}
// 		}
// 	);
// }
//
// USystemLinkAsyncPushSoftWidget* USystemLinkAsyncPushSoftWidget::PushSoftWidgetToStackAsync(
// 	const UObject* WorldContextObject,
// 	APlayerController* OwningPlayerController,
// 	TSoftClassPtr<USystemLinkActivatableWidget> InSoftWidgetClass,
// 	ESystemLinkWidgetStackType StackType,
// 	bool bFocusOnNewlyPushedWidget
// )
// {
// 	checkf(!InSoftWidgetClass.IsNull(), TEXT("PushSoftWidgetToStack was passed a null soft widget class "));
//
// 	if (GEngine)
// 	{
// 		if (UWorld* World = GEngine->
// 			GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
// 		{
// 			USystemLinkAsyncPushSoftWidget* Node = NewObject<USystemLinkAsyncPushSoftWidget>();
// 			Node->CachedOwningWorld = World;
// 			Node->CachedOwningPC = OwningPlayerController;
// 			Node->CachedSoftWidgetClass = InSoftWidgetClass;
// 			Node->CachedStackType = StackType;
// 			Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;
//
// 			Node->RegisterWithGameInstance(World);
//
// 			return Node;
// 		}
// 	}
//
// 	return nullptr;
// }

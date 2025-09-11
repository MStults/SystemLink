#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "SystemLinkUITypes.h"
#include "SystemLink/SystemLinkPlayerController.h"
#include "SystemLinkConfirmAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModalButtonClicked, EConfirmScreenButtonType, ButtonType);

UCLASS()
class SYSTEMLINK_API USystemLinkConfirmAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnModalButtonClicked Completed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static USystemLinkConfirmAsyncAction* ConfirmUsingControllerAsync(
		ASystemLinkPlayerController* Controller,
		EConfirmScreenType ConfirmScreenType,
		const FString& InScreenTitle,
		const FString& InScreenMsg);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static USystemLinkConfirmAsyncAction* ConfirmAsync(
		USystemLinkActivatableWidget* ActivatableWidget,
		EConfirmScreenType ConfirmScreenType,
		const FString& InScreenTitle,
		const FString& InScreenMsg);

	virtual void Activate() override;
private:
	UPROPERTY()
	ASystemLinkPlayerController* CachedController;

	EConfirmScreenType CachedType;
	FString CachedTitle;
	FString CachedMsg;

	void HandleModalClosed(EConfirmScreenButtonType ButtonType);
};

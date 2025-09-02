// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CommonRotator.h"
#include "BeepSimpleRotator.generated.h"

/**
 * 
 */
UCLASS()
class SYSTEMLINK_API UBeepSimpleRotator : public UCommonRotator
{
	GENERATED_BODY()
public:
	// Populate options so they show in designer and at runtime
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void HandleClicked();
};
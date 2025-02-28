// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SystemLinkFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SYSTEMLINK_API USystemLinkFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	/**
	 * Prints a debug message to the screen and log, with multiplayer support.
	 *
	 * @param WorldContextObject
	 * @param Messages The array of strings to append together.
	 * @param TextColor The color of the text on the screen.
	 * @param Duration How long the message should stay on screen.
	 * @param Key A unique key for the message (same key replaces previous messages).
	 * @param bLogToFile Whether to log this message to a file.
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "System Link Debug")
	static void PrintDebugMessage(
		UObject* WorldContextObject,
		const TArray<FString>& Messages,
		FColor TextColor = FColor::Green,
		float Duration = 5.0f,
		int32 Key = -1,
		bool bLogToFile = false
	);	
};

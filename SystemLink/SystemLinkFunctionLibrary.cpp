// Fill out your copyright notice in the Description page of Project Settings.


#include "SystemLinkFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"

void USystemLinkFunctionLibrary::PrintDebugMessage(
	UObject* WorldContextObject,
	const TArray<FString>& Messages,
	const FColor TextColor,
	const float Duration,
	const int32 Key,
	const bool bLogToFile)
{
	if (!WorldContextObject) return;

	// Combine messages into a single string
	FString FinalMessage;
	for (const FString& Msg : Messages)
	{
		FinalMessage += Msg + TEXT(" ");
	}

	// Ensure we are in a valid world
	const UWorld* World = WorldContextObject->GetWorld();
	if (!World) return;

	// Get the local player controller (for client-side messaging)
	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

	// Check if this is the server or a client

	// Server should log and print only for itself
	if (World->GetNetMode() != NM_Client)
	{
		// Print to server console log
		UE_LOG(LogTemp, Warning, TEXT("[SERVER] %s"), *FinalMessage);

		// Print to screen (for server only)
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(Key, Duration, TextColor, FString("[SERVER] ") + FinalMessage);
		}
	}
	else if (PC) // Client: Only print on the owning client
	{
		UE_LOG(LogTemp, Warning, TEXT("[CLIENT] %s"), *FinalMessage);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(Key, Duration, TextColor, FString("[CLIENT] ") + FinalMessage);
		}
	}

	// Optional: Log message to a file
	if (bLogToFile)
	{
		const FString LogDirectory = FPaths::ProjectLogDir();
		const FString FilePath = LogDirectory + TEXT("DebugMessages.log");

		FString FileText = FinalMessage + LINE_TERMINATOR;
		FFileHelper::SaveStringToFile(FileText, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
	}
}

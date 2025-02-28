// SystemLinkGameMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SystemLinkGameMode.generated.h"

UCLASS()
class ASystemLinkGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASystemLinkGameMode();

	// Expose the default pawn class so it can be set in the editor
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Game Mode")
	TSubclassOf<APawn> CustomDefaultPawnClass;

	// Expose the HUD class so it can be set in the editor
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Game Mode")
	TSubclassOf<class AHUD> CustomHUDClass;
};

// SystemLinkGameMode.cpp
#include "SystemLinkGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/HUD.h"

ASystemLinkGameMode::ASystemLinkGameMode()
{
	// Setup Default Pawn Class
	if (!CustomDefaultPawnClass)
	{
		static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
		if (PlayerPawnBPClass.Class != nullptr)
		{
			CustomDefaultPawnClass = PlayerPawnBPClass.Class;
		}
	}
	DefaultPawnClass = CustomDefaultPawnClass;

	// Setup HUD Class
	if (!CustomHUDClass)
	{
		// Optionally, you can provide a default HUD class if you have one. For example:
		static ConstructorHelpers::FClassFinder<AHUD> HudBPClass(TEXT("/Game/UI/BP_DefaultHUD"));
		if (HudBPClass.Class != nullptr)
		{
			CustomHUDClass = HudBPClass.Class;
		}
	}
	HUDClass = CustomHUDClass;
}

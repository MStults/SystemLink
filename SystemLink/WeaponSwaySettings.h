#pragma once

#include "CoreMinimal.h"
#include "WeaponSwaySettings.generated.h"

/** Struct to define weapon sway settings */
USTRUCT(BlueprintType)
struct FWeaponSwaySettings
{
	GENERATED_BODY()

public:
	/** How much the weapon sways based on input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway")
	float SwayStrength = 2.0f;

	/** How fast the sway interpolates */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway")
	float InterpSpeed = 5.0f;

	/** Maximum yaw rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway")
	float MaxYaw = 15.0f;

	/** Maximum pitch rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway")
	float MaxPitch = 10.0f;

	/** Default constructor */
	FWeaponSwaySettings()
	{
		SwayStrength = 2.0f;
		InterpSpeed = 5.0f;
		MaxYaw = 15.0f;
		MaxPitch = 10.0f;
	}
};

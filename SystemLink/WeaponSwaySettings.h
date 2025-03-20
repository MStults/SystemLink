#pragma once

#include "CoreMinimal.h"
#include "WeaponSwaySettings.generated.h"

/** Struct to define weapon sway settings */
USTRUCT(BlueprintType)
struct FWeaponSwaySettings
{
	GENERATED_BODY()

public:
	/** ===== General Sway Settings ===== */

	/** How much the weapon sways based on input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway | General")
	float SwayStrength = 2.0f;

	/** How fast the sway interpolates */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway | General")
	float InterpSpeed = 5.0f;

	/** Maximum yaw rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway | General")
	float MaxYaw = 15.0f;

	/** Maximum pitch rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway | General")
	float MaxPitch = 10.0f;

	/** Maximum roll limit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway | General")
	float MaxRoll = 5.0f;

	/** Multiplier applied to all sway calculations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway | General")
	float SwayMultiplier = 1.0f;

	/** ===== Breathing & Roll Settings ===== */

	/** Frequency of breathing roll */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway | Breathing & Roll")
	float BreathingRollSpeed = 1.5f;

	/** Intensity of breathing roll */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway | Breathing & Roll")
	float BreathingRollIntensity = 2.0f;

	/** How much movement affects roll */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway | Breathing & Roll")
	float MovementRollMultiplier = 0.5f;

	/** ===== Movement Sway Settings ===== */

	/** How much movement speed influences the overall sway */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway | Movement")
	float MovementSwayStrength = 1.0f;

	/** ===== Weapon Bobbing Settings ===== */

	/** Controls overall intensity of the bobbing effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway | Bobbing")
	float BobbingStrength = 1.0f;

	/** How fast the bobbing occurs (higher = faster bobbing) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway | Bobbing")
	float BobbingSpeed = 10.0f;

	/** Maximum vertical offset (up and down movement) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway | Bobbing")
	float BobbingAmplitude = 2.0f;

	/** Maximum horizontal offset (side to side movement) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway | Bobbing")
	float BobbingHorizontalAmplitude = 1.0f;

	/** ===== Constructor ===== */
	FWeaponSwaySettings() {}
};

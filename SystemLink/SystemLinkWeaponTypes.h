#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "SystemLinkWeaponTypes.generated.h"

/** Struct for shot information used during prediction and reconciliation */
USTRUCT(BlueprintType)
struct FShotInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "SystemLink|Shooting")
	FVector StartLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "SystemLink|Shooting")
	FVector EndLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "SystemLink|Shooting")
	float Timestamp = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "SystemLink|Shooting")
	FVector Direction = FVector::ForwardVector;

	UPROPERTY(BlueprintReadOnly, Category = "SystemLink|Shooting")
	FRotator Rotation = FRotator::ZeroRotator;

	FShotInfo() {}

	FShotInfo(const FVector& InStart, const FVector& InEnd, const float InTimestamp)
		: StartLocation(InStart)
		, EndLocation(InEnd)
		, Timestamp(InTimestamp)
	{
		Direction = (EndLocation - StartLocation).GetSafeNormal();
		Rotation = Direction.Rotation();
	}
};

/** Struct to define weapon sway settings */
USTRUCT(BlueprintType)
struct FWeaponSwaySettings
{
	GENERATED_BODY()
	
	/** ===== General Sway Settings ===== */

	UPROPERTY(BlueprintReadWrite, Category = "Weapon|Sway")
	bool bIsSwayEnabled = true;

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

/** Struct for defining weapon spread settings */
USTRUCT(BlueprintType)
struct FWeaponSpreadSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Spread")
	float BaseConeAngle = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Spread")
	int32 NumPellets = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Spread")
	float MaxRange = 10000.0f;
};

USTRUCT(BlueprintType)
struct FConfirmedProjectileHit
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FShotInfo Shot;

	UPROPERTY(BlueprintReadOnly)
	FHitResult Hit;
};
#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "WeaponSwaySettings.h"
#include "SystemLinkWeaponComponent.generated.h"

UCLASS()
class SYSTEMLINK_API USystemLinkWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
public:
	USystemLinkWeaponComponent();
	
public:
	// Allows Blueprint to call these functions
	UFUNCTION(BlueprintCallable, Category = "Weapon|Effects",  meta = (CallInEditor = "true"))
	void SpawnImpactDecal(const FHitResult& HitResult) const;

	UFUNCTION(BlueprintCallable, Category = "Weapon|Effects", meta = (CallInEditor = "true"))
	void SpawnImpactEffect(const FHitResult& HitResult) const;
	
	/**
	 * Generates end points for a bullet spread using a cone angle.
	 *
	 * @param StartLocation The origin of the blast.
	 * @param EndLocation The base end location before applying spread.
	 * @param NumPellets Number of pellets/bullets in the blast.
	 * @param ConeAngle The half-angle of the spread cone in degrees.
	 * @param OutPelletEndPoints Output array of calculated end points.
	 */
	UFUNCTION(BlueprintCallable, Category = "SystemLink|Shooting")
	static void GenerateBulletSpread(
		const FVector& StartLocation,
		const FVector& EndLocation,
		int32 NumPellets,
		float ConeAngle,
		TArray<FVector>& OutPelletEndPoints
	);

	/**
	 * Calculates the start and end locations for a weapon trace.
	 *
	 * @param CameraLocation The world position of the player's camera.
	 * @param CameraForwardVector The forward vector of the camera (direction the player is looking).
	 * @param SocketName The name of the socket on the weapon (e.g., "Muzzle").
	 * @param TraceDistance The max range for the shot.
	 * @param OutStartLocation (Output) The calculated start location of the shot (weapon muzzle position).
	 * @param OutEndLocation (Output) The calculated end location of the shot (hit target or max range).
	 * @return True if the trace hit something, false if no hit.
	 */
	UFUNCTION(BlueprintCallable, Category = "SystemLink|Shooting")
	bool GetShotStartAndEnd(
		const FVector& CameraLocation, 
		const FVector& CameraForwardVector, 
		FName SocketName, 
		float TraceDistance, 
		FVector& OutStartLocation, 
		FVector& OutEndLocation
	) const;

	/** Sway settings for this weapon (editable in Blueprints) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway")
	FWeaponSwaySettings SwaySettings;

	/** Calculates the sway rotation based on input */
	UFUNCTION(BlueprintCallable, Category = "Weapon Sway")
	FRotator CalculateWeaponSway(float LookX, float LookY, float DeltaTime, float PlayerSpeed);

	// Function to check if the weapon is blocked by a wall
	UFUNCTION(BlueprintCallable, Category = "SystemLink|Weapon")
	bool CheckWeaponBlocking(const FVector& CameraLocation, const FVector& CameraForwardVector, const bool bDebug = false);
	
protected:
	/** Stores the last frame’s sway rotation */
	FRotator SwayRotation;
	
	// Decal material (assign this in Blueprint)
	UPROPERTY(EditDefaultsOnly, Category = "SystemLink|Shooting")
	UMaterialInterface* ImpactDecalMaterial;

	// Default particle effect for impacts (assign in Blueprint)
	UPROPERTY(EditDefaultsOnly, Category = "SystemLink|Shooting")
	UNiagaraSystem* ImpactNiagaraEffect;

	// The maximum distance at which the weapon will perform a blocking trace check to detect nearby obstacles.
	// If an obstacle is detected within this range, the weapon will be considered "blocked," triggering animations
	// or positional adjustments to prevent clipping through walls or objects. This value is adjustable in Blueprints.
	UPROPERTY(EditDefaultsOnly, Category = "SystemLink|Shooting")
	float WeaponBLockingTraceDistance = 30;

private:
	// Tracks if the weapon is blocked
	bool bIsWeaponBlocked;
};

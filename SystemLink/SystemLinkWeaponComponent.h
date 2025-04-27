#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponTypes.h"
#include "SystemLinkWeaponComponent.generated.h"

UCLASS()
class SYSTEMLINK_API USystemLinkWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

	// Timer handle for shot validation
	FTimerHandle ShotValidationTimerHandle;
	
public:
	USystemLinkWeaponComponent();

	// ============================
	// === Weapon Multiplayer ===
	// ============================

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Details")
	int32 NumProjectiles = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Details")
	float ConeAngle = 2.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Details")
	float BaseDamage = 20.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Details")
	float TraceDistance = 25000.0f;
	
	/** Editable sway settings exposed to Blueprint */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details")
	FWeaponSwaySettings SwaySettings;

	/** Maximum distance for checking if the weapon is blocked */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Details")
	float WeaponBlockingTraceDistance = 30.f;

	/** Internal state tracking whether the weapon is currently blocked */
	UPROPERTY(VisibleAnywhere, Category = "Weapon Details")
	bool bIsWeaponBlocked;

	/** Tracks the history of fired shots for validation */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Details")
	TArray<FShotInfo> ShotHistory;

	/** 
	 * Number of locally predicted shots to accumulate before sending validation to the server. 
	 * Used to control how frequently the client sends shot history.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details")
	int32 MaxUnvalidatedShots = 1; // Default — adjust per weapon

	/** 
	 * The maximum time (in seconds) to wait before sending unvalidated shots to the server for reconciliation.
	 * 
	 * This timer ensures that even if the MaxUnvalidatedShots threshold is not reached (e.g., during slow firing),
	 * the client will still send its predicted shots after this interval. 
	 * 
	 * Recommended to keep this value short (e.g., 0.1 to 0.2 seconds) to maintain responsiveness 
	 * and minimize desync in multiplayer.
	 * 
	 * Used in conjunction with MaxUnvalidatedShots to trigger ServerValidateShots().
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Details")
	float ShotValidationInterval = 0.15f;

	/** 
	 * Sweep radius used for fallback hit validation on the server.
	 * A higher value gives more forgiveness for fast movement, latency, or wide projectiles (like shotgun pellets).
	 * Set to 0 for strict line traces only.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Details")
	float HitValidationSweepRadius = 15.f;

	/** Stores the last calculated sway rotation */
	UPROPERTY(VisibleAnywhere, Category = "Weapon Details")
	FRotator SwayRotation;

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon|Shooting") 
	void OnProjectileHitPredicted(const TArray<FHitResult>& Hits);

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon|Shooting")
	void OnProjectileHitConfirmed(const TArray<FConfirmedProjectileHit>& ConfirmedHits);

	UFUNCTION(BlueprintCallable, Category = "Weapon|Shooting")
	static TArray<FHitResult> ExtractHitResults(const TArray<FConfirmedProjectileHit>& ConfirmedHits);

	/** Client-side local fire prediction */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon|Shooting")
	void LocalFire(const FVector& StartLocation, const FVector& EndLocation);
	virtual void LocalFire_Implementation(const FVector& StartLocation,	const FVector& EndLocation);

	/** Server-side validation of client-reported shots */
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Weapon|Multiplayer")
	void ServerValidateShots(const TArray<FShotInfo>& Shots);

	/** Sends accumulated shot data for server validation */
	UFUNCTION(BlueprintCallable, Category = "Weapon|Multiplayer")
	void SendShotValidation();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayFireFX();
	void MulticastPlayFireFX_Implementation();

	/** Called when fire FX should be played on all clients (muzzle flash, sound, etc.) */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon|Shooting")
	void OnMulticastPlayFireFX();

	// =====================
	// === Weapon Sway ===
	// =====================

	/** Calculates sway based on player input */
	UFUNCTION(BlueprintCallable, Category = "Weapon|Sway")
	FRotator CalculateWeaponSway(float LookX, float LookY, float DeltaTime, float PlayerSpeed);

	// ========================
	// === Weapon Shooting ===
	// ========================

	/** Calculates start/end locations for a shot trace */
	UFUNCTION(BlueprintCallable, Category = "Weapon|Shooting")
	bool GetShotStartAndEnd(
		const FVector& CameraLocation, 
		const FVector& CameraForwardVector, 
		FName SocketName, 
		FVector& OutStartLocation, 
		FVector& OutEndLocation
	) const;

	/** Generates multiple spread shot endpoints */
	UFUNCTION(BlueprintCallable, Category = "Weapon|Shooting")
	void GenerateBulletSpread(
		const FVector& StartLocation,
		const FVector& EndLocation,
		TArray<FVector>& OutProjectilesEndPoints
	);

	// ============================
	// === Weapon Blocking ===
	// ============================

	/** Checks if the weapon is blocked by environment geometry */
	UFUNCTION(BlueprintCallable, Category = "Weapon|Blocking")
	bool CheckWeaponBlocking(const FVector& CameraLocation, const FVector& CameraForwardVector, bool bDebug = false);


	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugPrintScreen(const FString& Message, FColor Color = FColor::White, float Duration = 2.0f) const;
};

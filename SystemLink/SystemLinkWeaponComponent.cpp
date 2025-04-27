#include "SystemLinkWeaponComponent.h"

#include "SystemLink.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

USystemLinkWeaponComponent::USystemLinkWeaponComponent()
{
	bIsWeaponBlocked = false;
	WeaponBlockingTraceDistance = 30.0f; // Adjust as needed
	PrimaryComponentTick.bCanEverTick = true; // Needed for weapon animations?
	SwayRotation = FRotator::ZeroRotator;
}

TArray<FHitResult> USystemLinkWeaponComponent::ExtractHitResults(const TArray<FConfirmedProjectileHit>& ConfirmedHits)
{
	TArray<FHitResult> OutHits;
	OutHits.Reserve(ConfirmedHits.Num());

	for (const FConfirmedProjectileHit& Confirmed : ConfirmedHits)
	{
		OutHits.Add(Confirmed.Hit);
	}

	return OutHits;
}

void USystemLinkWeaponComponent::LocalFire_Implementation(const FVector& StartLocation,	const FVector& EndLocation)
{
	// DebugPrintScreen(TEXT("🔥 Entered LocalFire_Implementation"), FColor::Yellow, 3.0f);
	if (!GetWorld() || !GetOwner()) return;

	// DebugPrintScreen(TEXT("📦 Generating bullet spread"));
	// DebugPrintScreen(FString::Printf(TEXT("🧮 GenerateBulletSpread called")), FColor::Yellow);
	// DebugPrintScreen(FString::Printf(TEXT("→ NumProjectiles: %d"), NumProjectiles), FColor::Yellow);
	// DebugPrintScreen(FString::Printf(TEXT("→ ConeAngle: %f"), ConeAngle), FColor::Yellow);
	
	// Use your own spread method to calculate endpoints
	TArray<FVector> BulletEndPoints;
	GenerateBulletSpread(StartLocation, EndLocation, BulletEndPoints);
	TArray<FHitResult> PredictedHits;

	// DebugPrintScreen(FString::Printf(TEXT("🔫 Num EndPoints: %d"), BulletEndPoints.Num()), FColor::Cyan);

	for (const FVector& BulletEnd : BulletEndPoints)
	{
		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetOwner());

		// Local prediction trace
		GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, BulletEnd, COLLISION_WEAPON, Params);

		// Add to reconciliation list
		ShotHistory.Add(FShotInfo(StartLocation, BulletEnd, GetWorld()->GetTimeSeconds()));		

		if (Hit.bBlockingHit)
		{
			PredictedHits.Add(Hit);
		}
	}

	if (PredictedHits.Num() > 0)
    {
    	OnProjectileHitPredicted(PredictedHits);
    }
	
	// 🔁 Flush if threshold is reached
	if (ShotHistory.Num() >= MaxUnvalidatedShots)
	{
		SendShotValidation();
	}

	if (!GetWorld()->GetTimerManager().IsTimerActive(ShotValidationTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(
			ShotValidationTimerHandle,
			this,
			&USystemLinkWeaponComponent::SendShotValidation,
			ShotValidationInterval,
			false
		);
	}
}

void USystemLinkWeaponComponent::SendShotValidation()
{
	GetWorld()->GetTimerManager().ClearTimer(ShotValidationTimerHandle);
	
	if (ShotHistory.Num() > 0)
	{
		ServerValidateShots(ShotHistory);
		ShotHistory.Empty();
	}
}

void USystemLinkWeaponComponent::ServerValidateShots_Implementation(const TArray<FShotInfo>& Shots)
{
	MulticastPlayFireFX();

	TArray<FConfirmedProjectileHit> ConfirmedHits;

	for (const FShotInfo& Shot : Shots)
	{
		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetOwner());

		const FVector Start = Shot.StartLocation;
		const FVector End = Shot.EndLocation;

		// Try precise line trace first
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, COLLISION_WEAPON, Params);

		// If line trace fails, try sweep as fallback
		if (!bHit)
		{
			FCollisionShape Sphere = FCollisionShape::MakeSphere(HitValidationSweepRadius);

			bHit = GetWorld()->SweepSingleByChannel(
				Hit,
				Start,
				End,
				FQuat::Identity,
				COLLISION_WEAPON,
				Sphere,
				Params
			);
		}

		// Accept if either trace hit
		if (bHit)
		{
			FConfirmedProjectileHit Confirmed;
			Confirmed.Shot = Shot;
			Confirmed.Hit = Hit;
			ConfirmedHits.Add(Confirmed);
		}
	}

	if (ConfirmedHits.Num() > 0)
	{
		// DebugPrintScreen(FString::Printf(TEXT("ConfirmedHits: %d"), ConfirmedHits.Num()), FColor::Orange);
		OnProjectileHitConfirmed(ConfirmedHits);
	}
}

bool USystemLinkWeaponComponent::ServerValidateShots_Validate(const TArray<FShotInfo>& Shots)
{
	return true; // or add basic validation later
}

void USystemLinkWeaponComponent::MulticastPlayFireFX_Implementation()
{
	OnMulticastPlayFireFX();
}

void USystemLinkWeaponComponent::GenerateBulletSpread(
	const FVector& StartLocation,
	const FVector& EndLocation,
	TArray<FVector>& OutProjectilesEndPoints)
{
	// Clear output array
	OutProjectilesEndPoints.Empty();
 
	// Calculate base direction
	const FVector BaseDirection = (EndLocation - StartLocation).GetSafeNormal();
 
	// Convert cone angle to radians
	const float ConeAngleRad = FMath::DegreesToRadians(ConeAngle);

	for (int32 i = 0; i < NumProjectiles; i++)  
	{
		// Generate a random direction within the cone
		FVector SpreadDirection = UKismetMathLibrary::RandomUnitVectorInConeInRadians(BaseDirection, ConeAngleRad);

		// Scale the spread vector to match the intended shot distance
		FVector PelletEnd = StartLocation + (SpreadDirection * TraceDistance);

		// Add to output array
		OutProjectilesEndPoints.Add(PelletEnd);
	}
}

bool USystemLinkWeaponComponent::GetShotStartAndEnd(
	const FVector& CameraLocation,
	const FVector& CameraForwardVector,
	const FName SocketName,
	FVector& OutStartLocation,
	FVector& OutEndLocation) const
{
	// Compute the end point for the camera trace
	const FVector TraceEnd = CameraLocation + (CameraForwardVector * TraceDistance);

	// Set up trace parameters
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner()); // Ignore the weapon owner

	FHitResult HitResult;

	// Perform a line trace from the camera
	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		CameraLocation,
		TraceEnd,
		ECC_Visibility, // Collision channel
		QueryParams
	);

	// Use the hit location if we hit something, otherwise use max range
	OutEndLocation = bHit ? HitResult.ImpactPoint : TraceEnd;

	// Ensure the weapon has the specified socket
	if (DoesSocketExist(SocketName))
	{
		// Get the socket location on the weapon (muzzle position)
		OutStartLocation = GetSocketLocation(SocketName);
		return bHit; // Return whether we hit something
	}

	// Fallback: If the socket does not exist, use the camera location as the start
	OutStartLocation = CameraLocation;
	return bHit;
}

FRotator USystemLinkWeaponComponent::CalculateWeaponSway(const float LookX, const float LookY, const float DeltaTime,
                                                         const float PlayerSpeed)
{
	// Calculate target sway rotation from mouse/controller input
	FRotator TargetRotation;
	TargetRotation.Pitch = FMath::Clamp(LookY * -SwaySettings.SwayStrength, -SwaySettings.MaxPitch,
	                                    SwaySettings.MaxPitch);
	TargetRotation.Yaw = FMath::Clamp(LookX * SwaySettings.SwayStrength, -SwaySettings.MaxYaw, SwaySettings.MaxYaw);

	// Add procedural breathing roll
	const float Time = GetWorld()->GetTimeSeconds();
	const float BreathingRoll = FMath::Sin(Time * SwaySettings.BreathingRollSpeed) * SwaySettings.
		BreathingRollIntensity;

	// Add roll based on horizontal movement
	const float MovementRoll = LookX * -SwaySettings.SwayStrength * SwaySettings.MovementRollMultiplier;
	
	// Combine roll effects
	TargetRotation.Roll = FMath::Clamp(BreathingRoll + MovementRoll, -SwaySettings.MaxRoll, SwaySettings.MaxRoll);

	// 🌟 Apply Weapon Bobbing Effect 🌟
	if (const float NormalizedSpeed = FMath::Clamp(PlayerSpeed / 600.0f, 0.0f, 2.0f); NormalizedSpeed > 0.1f)
	// Only apply bobbing if the player is moving
	{
		const float BobbingOffset = FMath::Sin(Time * SwaySettings.BobbingSpeed * NormalizedSpeed) * SwaySettings.
			BobbingAmplitude * NormalizedSpeed;
		
		const float BobbingSideOffset = FMath::Cos(Time * SwaySettings.BobbingSpeed * NormalizedSpeed * 0.5f) *
			SwaySettings.BobbingHorizontalAmplitude * NormalizedSpeed;

		// Apply bobbing offsets to Pitch and Yaw for better visual feel
		TargetRotation.Pitch += BobbingOffset;
		TargetRotation.Yaw += BobbingSideOffset;
	}

	// Apply Sway Multiplier (Now applied as a final scaling factor)
	TargetRotation *= SwaySettings.SwayMultiplier;

	// Smoothly interpolate towards target sway
	SwayRotation = FMath::RInterpTo(SwayRotation, TargetRotation, DeltaTime, SwaySettings.InterpSpeed);

	return SwayRotation;
}

bool USystemLinkWeaponComponent::CheckWeaponBlocking(const FVector& CameraLocation, const FVector& CameraForwardVector, const bool bDebug)
{
	const FVector TraceStart = CameraLocation;
	const FVector TraceEnd = TraceStart + (CameraForwardVector * WeaponBlockingTraceDistance);
	constexpr float SphereRadius = 15.0f; // Adjust as needed

	FCollisionQueryParams TraceParams(FName(TEXT("WeaponBlockTrace")), true, GetOwner());
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActor(GetOwner());

	FHitResult HitResult;
	const bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(SphereRadius),
		TraceParams
	);

#if WITH_EDITOR
	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), TraceEnd, SphereRadius, 12, bHit ? FColor::Red : FColor::Green, false, 1.0f);
	}
#endif

	bIsWeaponBlocked = bHit;
	return bIsWeaponBlocked;
}

void USystemLinkWeaponComponent::DebugPrintScreen(const FString& Message, const FColor Color, const float Duration) const
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			10,
			Color,
			Message
		);
	}
}


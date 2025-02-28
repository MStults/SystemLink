#include "SystemLinkWeaponComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void USystemLinkWeaponComponent::SpawnImpactDecal(const FHitResult& HitResult) const
{
	if (!ImpactDecalMaterial) return;

	UGameplayStatics::SpawnDecalAtLocation(
		GetWorld(),
		ImpactDecalMaterial,
		FVector(10.0f, 10.0f, 1.0f),   // Decal Size
		HitResult.ImpactPoint,         // Spawn Location
		HitResult.ImpactNormal.Rotation(), // Rotation based on surface
		10.0f                           // Lifespan in seconds
	);
}

void USystemLinkWeaponComponent::SpawnImpactEffect(const FHitResult& HitResult) const
{
	if (!ImpactNiagaraEffect) return;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		ImpactNiagaraEffect,
		HitResult.ImpactPoint,
		HitResult.ImpactNormal.Rotation()
	);
}

void USystemLinkWeaponComponent::GenerateBulletSpread(
	const FVector& StartLocation,
	const FVector& EndLocation,
	const int32 NumPellets,
	const float ConeAngle, TArray<FVector>& OutPelletEndPoints)
{
	// Clear output array
	OutPelletEndPoints.Empty();

	// Calculate base direction
	const FVector BaseDirection = (EndLocation - StartLocation).GetSafeNormal();

	// Convert cone angle to radians
	const float ConeAngleRad = FMath::DegreesToRadians(ConeAngle);

	for (int32 i = 0; i < NumPellets; i++)
	{
		// Generate a random direction within the cone
		FVector SpreadDirection = UKismetMathLibrary::RandomUnitVectorInConeInRadians(BaseDirection, ConeAngleRad);

		// Scale the spread vector to match the intended shot distance
		const float TraceDistance = (EndLocation - StartLocation).Size();
		FVector PelletEnd = StartLocation + (SpreadDirection * TraceDistance);

		// Add to output array
		OutPelletEndPoints.Add(PelletEnd);
	}
}


bool USystemLinkWeaponComponent::GetShotStartAndEnd(
	const FVector& CameraLocation, 
	const FVector& CameraForwardVector,
	const FName SocketName, 
	const float TraceDistance, 
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

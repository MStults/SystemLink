#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "ShotDetectorComponent.generated.h"

// Standard replication event (triggers on clients via OnRep)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShotDetected, FVector, HitLocation);

// Multicast event (triggers instantly via NetMulticast)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShotDetectedMulticast, FVector, HitLocation);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SYSTEMLINK_API UShotDetectorComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UShotDetectorComponent();

	/** Event triggered when a shot (trace hit) is detected (Replicated) */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnShotDetected OnShotDetected;

	/** Event triggered when a shot is detected using a multicast RPC (Optional) */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnShotDetectedMulticast OnShotDetectedMulticast;

	/** Called when a shot (trace hit) is detected */
	UFUNCTION(BlueprintCallable, Category = "Shot Detection")
	void HandleTraceHit(const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;

private:
	/** Last recorded hit location, replicated */
	UPROPERTY(ReplicatedUsing = OnRep_ShotDetected)
	FVector LastHitLocation;

	/** Notifies clients when `LastHitLocation` updates */
	UFUNCTION()
	void OnRep_ShotDetected();

	/** Server function to update `LastHitLocation` */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerHandleTraceHit(const FHitResult& Hit);

	/** Multicast function to instantly sync the shot event across all clients */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastShotDetected(FVector HitLocation);

	/** Ensures properties are replicated */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

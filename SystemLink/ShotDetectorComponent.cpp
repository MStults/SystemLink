#include "ShotDetectorComponent.h"

#include "SystemLink.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"

UShotDetectorComponent::UShotDetectorComponent()
{
    SetIsReplicatedByDefault(true);

    // Enable collision
    SetGenerateOverlapEvents(true);
    UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    UPrimitiveComponent::SetCollisionObjectType(ECC_WorldDynamic);
    UPrimitiveComponent::SetCollisionResponseToAllChannels(ECR_Ignore);
    UPrimitiveComponent::SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Block);
}

void UShotDetectorComponent::BeginPlay()
{
    Super::BeginPlay();
}

/** Called when a shot hits this component */
void UShotDetectorComponent::HandleTraceHit(const FHitResult& Hit)
{
    if (Hit.Component == this)
    {
        if (!GetOwner()->HasAuthority())
        {
            ServerHandleTraceHit(Hit);
            return;
        }       

        // Update the replicated hit location (triggers OnRep)
        LastHitLocation = Hit.ImpactPoint;

        // Call `OnRep_ShotDetected` on the server
        OnRep_ShotDetected();

        MulticastShotDetected(Hit.ImpactPoint); 
    }
}

/** Called on all clients when `LastHitLocation` is updated */
void UShotDetectorComponent::OnRep_ShotDetected()
{
    UE_LOG(LogTemp, Warning, TEXT("Replicated Shot Detected at: %s"), *LastHitLocation.ToString());

    if (OnShotDetected.IsBound())
    {
        OnShotDetected.Broadcast(LastHitLocation);
    }
}

/** Multicast event for real-time effects */
void UShotDetectorComponent::MulticastShotDetected_Implementation(FVector HitLocation)
{
    UE_LOG(LogTemp, Warning, TEXT("Multicast Shot Detected at: %s"), *HitLocation.ToString());

    if (OnShotDetectedMulticast.IsBound())
    {
        OnShotDetectedMulticast.Broadcast(HitLocation);
    }
}

/** Ensures replication happens for `LastHitLocation` */
void UShotDetectorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UShotDetectorComponent, LastHitLocation);
}

/** Server RPC to handle trace hits */
void UShotDetectorComponent::ServerHandleTraceHit_Implementation(const FHitResult& Hit)
{
    HandleTraceHit(Hit);
}

bool UShotDetectorComponent::ServerHandleTraceHit_Validate(const FHitResult& Hit)
{
    return true;
}

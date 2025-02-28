#pragma once

#include "CoreMinimal.h"
#include "HealthTypes.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

// Delegate for broadcasting health and shield changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, CurrentShield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShieldDepleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOvershieldActivated, bool, bIsCharging);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShieldRechargeStarted, float, RechargeDuration);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SYSTEMLINK_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

protected:
    virtual void BeginPlay() override;

    /** Whether the shield is currently recharging */
    UPROPERTY(ReplicatedUsing = OnRep_IsRecharging)
    bool bIsRecharging;

    /** Current Shield, replicated */
    UPROPERTY(ReplicatedUsing = OnRep_Shield, EditAnywhere, BlueprintReadOnly, Category = "Health")
    float Shield;

    /** Max Shield */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
    float MaxShield;

    /** Current Health, replicated */
    UPROPERTY(ReplicatedUsing = OnRep_Health, EditAnywhere, BlueprintReadOnly, Category = "Health")
    float Health;

    /** Max Health */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
    float MaxHealth;

    /** Shield recharge delay after taking damage */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
    float ShieldRechargeDelay;

    /** Shield recharge rate per second */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
    float ShieldRechargeRate;

    /** Overshield Variables */
    UPROPERTY(ReplicatedUsing = OnRep_Overshield, EditAnywhere, BlueprintReadOnly, Category = "Health")
    bool bHasOvershield;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
    float MaxOvershield;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
    float OvershieldChargeRate; // How fast it charges

    /** Timer handles */
    FTimerHandle ShieldRechargeTimer;
    FTimerHandle OvershieldChargeTimer;

    /** Functions */
    UFUNCTION()
    void OnRep_IsRecharging();
    
    UFUNCTION()
    void OnRep_Shield();

    UFUNCTION()
    void OnRep_Health();

    UFUNCTION()
    void OnRep_Overshield();

    void StartShieldRecharge();
    void RegenerateShield();

    void StartOvershieldCharge();
    void FinishOvershieldCharge();

    /** Server function to apply damage */
    UFUNCTION(Server, Reliable)
    void ServerApplyDamage(float DamageAmount);

public:
    /** Events for UI and effects */
    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnHealthChanged OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnShieldDepleted OnShieldDepleted;

    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnOvershieldActivated OnOvershieldActivated;

    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnShieldRechargeStarted OnShieldRechargeStarted;

    /** Apply damage */
    UFUNCTION(BlueprintCallable, Category = "Health")
    void ApplyDamage(float DamageAmount);

    /** Activate Overshield */
    UFUNCTION(BlueprintCallable, Category = "Health")
    void ActivateOvershield();

    /** Getters */
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetShield() const { return Shield; }

    UFUNCTION(BlueprintPure, Category = "Health")
    bool HasOvershield() const { return bHasOvershield; }
    
    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(float HealAmount);

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    bool bDamageEvent = false; // True only when ApplyDamage() is active
};

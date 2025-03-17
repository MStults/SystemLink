#include "HealthComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	MaxHealth = 100.f;
	Health = MaxHealth;

	MaxShield = 50.f;
	Shield = MaxShield;

	ShieldRechargeDelay = 3.0f;
	ShieldRechargeRate = 10.0f;

	bHasOvershield = false;
	MaxOvershield = 100.f; // Overshield is double normal shields
	OvershieldChargeRate = 20.0f; // 20 shields per second
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHealthComponent::Heal(const float HealAmount)
{
	if (!GetOwner()->HasAuthority()) return; // Server only

	Health = FMath::Clamp(Health + HealAmount, 0.0f, MaxHealth);
	OnRep_Health(); // Notify clients
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, Health);
	DOREPLIFETIME(UHealthComponent, Shield);
	DOREPLIFETIME(UHealthComponent, bHasOvershield);
	DOREPLIFETIME(UHealthComponent, bIsRecharging); 
}

void UHealthComponent::ApplyDamage(const float DamageAmount)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerApplyDamage(DamageAmount);
		return;
	}

	if (Health <= 0) return; // Dead, ignore further damage

	// Stop shield recharge if damage is taken
	GetWorld()->GetTimerManager().ClearTimer(ShieldRechargeTimer);
	bIsRecharging = false;

	// Handle shield damage
	if (Shield > 0)
	{
		Shield = FMath::Max(Shield - DamageAmount, 0.0f);
		if (Shield <= 0)
		{
			Health = FMath::Clamp(Health + Shield, 0.0f, MaxHealth);
			Shield = 0;
			MulticastOnShieldDepleted();
		}
	}
	else
	{
		Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	}

	TakeDamage(Health, Shield); // Call local damage event	

	// Restart shield recharge after delay
	GetWorld()->GetTimerManager().SetTimer(
		ShieldRechargeTimer,
		this,
		&UHealthComponent::StartShieldRecharge,
		ShieldRechargeDelay,
		false
	);
}

void UHealthComponent::ActivateOvershield()
{
	if (GetOwner()->HasAuthority() && Shield >= MaxShield) // Only start if shields are full
	{
		bHasOvershield = true;
		OnRep_Overshield();
		GetWorld()->GetTimerManager().SetTimer(
			OvershieldChargeTimer,
			this,
			&UHealthComponent::StartOvershieldCharge,
			0.1f,
			true);
	}
}

void UHealthComponent::StartOvershieldCharge()
{
	if (Shield < MaxOvershield)
	{
		Shield = FMath::Clamp(Shield + (OvershieldChargeRate * 0.1f), 0.0f, MaxOvershield);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(OvershieldChargeTimer);
		FinishOvershieldCharge();
	}
}

void UHealthComponent::FinishOvershieldCharge()
{
	Shield = FMath::Min(Shield, MaxShield); // Prevents overshield from sticking
	bHasOvershield = false;
	OnRep_Overshield();
}

void UHealthComponent::MulticastOnShieldDepleted_Implementation()
{
	OnShieldDepleted.Broadcast();
}

void UHealthComponent::ServerApplyDamage_Implementation(const float DamageAmount)
{
	ApplyDamage(DamageAmount);
}

void UHealthComponent::StartShieldRecharge()
{
	if (!GetOwner()->HasAuthority()) return; // Ensure this runs only on the server
	
	// Calculate remaining time to fully recharge the shield
	const float TimeToFullRecharge = (MaxShield - Shield) / ShieldRechargeRate;
    
	// Set flag and replicate to clients
	bIsRecharging = true;
	OnRep_IsRecharging(); // Call locally on the server
	OnShieldRechargeStarted.Broadcast(TimeToFullRecharge); // Server also triggers FX
    
	// Start the actual recharge
	GetWorld()->GetTimerManager().SetTimer(
		ShieldRechargeTimer, 
		this, 
		&UHealthComponent::RegenerateShield, 
		0.1f, 
		true
	);
}

void UHealthComponent::RegenerateShield()
{
	if (Shield < MaxShield)
	{
		Shield = FMath::Clamp(Shield + (ShieldRechargeRate * 0.1f), 0.0f, MaxShield);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(ShieldRechargeTimer);
		bIsRecharging = false;

		OnRep_Shield(); // Final shield update
		OnRep_IsRecharging();
	}      
}

void UHealthComponent::OnRep_Health()
{
	OnHealthChanged.Broadcast(Health, Shield);
}

void UHealthComponent::OnRep_IsRecharging()
{
	if (bIsRecharging)
	{
		const float TimeToFullRecharge = (MaxShield - Shield) / ShieldRechargeRate;
		OnShieldRechargeStarted.Broadcast(TimeToFullRecharge);
	} else
	{
		OnShieldRechargeComplete.Broadcast();
	}
}

void UHealthComponent::OnRep_Shield()
{
	if (bIsRecharging) return; // Ignore if recharging
	OnHealthChanged.Broadcast(Health, Shield);
}

void UHealthComponent::OnRep_Overshield()
{
	OnOvershieldActivated.Broadcast(bHasOvershield);
}

void UHealthComponent::TakeDamage_Implementation(const float NewHealth, const float NewShield)
{	
	// Assign the new values to the class variables
	Health = NewHealth;
	Shield = NewShield;

	// Broadcast event for UI updates
	OnHealthChanged.Broadcast(Health, Shield);
	OnTakeDamage.Broadcast(Health, Shield);
}


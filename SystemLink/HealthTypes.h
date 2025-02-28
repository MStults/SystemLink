#pragma once

#include "CoreMinimal.h"
#include "HealthTypes.generated.h"

USTRUCT(BlueprintType) // 🔥 This makes it work in Blueprints
struct FHealthState
{
	GENERATED_BODY() // 🔥 This allows Unreal to serialize it properly

	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth = 100.f;

	UPROPERTY(BlueprintReadOnly)
	float CurrentShield = 50.f;

	UPROPERTY(BlueprintReadOnly)
	bool bHealthDamaged = false;

	UPROPERTY(BlueprintReadOnly)
	bool bShieldDamaged = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsRecharging = false;

	UPROPERTY(BlueprintReadOnly)
	bool bHasOvershield = false;

	UPROPERTY(BlueprintReadOnly)
	float MaxHealth = 100.f;

	UPROPERTY(BlueprintReadOnly)
	float MaxShield = 50.f;

	UPROPERTY(BlueprintReadOnly)
	float MaxOvershield = 100.f;
};

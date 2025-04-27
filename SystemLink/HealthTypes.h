#pragma once

#include "CoreMinimal.h"
#include "HealthTypes.generated.h"

USTRUCT(BlueprintType)
struct FHealthChangedInfo
{
	GENERATED_BODY()  

	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth = 100.f;

	UPROPERTY(BlueprintReadOnly)
	float CurrentShield = 50.f;

	UPROPERTY(BlueprintReadOnly)
	bool bHealthDamaged = false;

	UPROPERTY(BlueprintReadOnly)
	bool bShieldDamaged = false;
};

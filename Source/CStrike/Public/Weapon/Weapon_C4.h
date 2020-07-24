// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon_Base.h"
#include "Weapon_C4.generated.h"

/**
 * 
 */

class AWeapon_C4_Actived;

USTRUCT()
struct FC4Data
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,Category = "Config")
	float ExplodeTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Config")
	float ExplodeRange;
};

UCLASS()
class CSTRIKE_API AWeapon_C4 : public AWeapon_Base
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Config")
	FC4Data C4Config;

	bool IsPlanting;

	FTimerHandle PlantTimer;

	FTimerHandle ExplodeTimer;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AWeapon_C4_Actived> PlantedC4;

public:

	// Check if player plant while moving
	virtual void Tick(float DeltaTime);

	// Start plant C4
	virtual void WeaponPrimaryFire() override;

	// Stop plant C4
	virtual void WeaponStopFire() override;

	// C4 plant finish
	UFUNCTION(BlueprintCallable, Category = "C4Action")
	void Planted();

	// C4 time is up ,explode
	void Explode();
};

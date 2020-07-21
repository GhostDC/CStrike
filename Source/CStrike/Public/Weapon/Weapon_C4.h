// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon_Base.h"
#include "Weapon_C4.generated.h"

/**
 * 
 */
UCLASS()
class CSTRIKE_API AWeapon_C4 : public AWeapon_Base
{
	GENERATED_BODY()

public:
	virtual void WeaponPrimaryFire() override;

	virtual void WeaponStopFire() override;

	void Planted();

	void Explode();
};

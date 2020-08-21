// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon_Base.h"
#include "Weapon_Grenade.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class AWeapon_GrenadeProjectile;
/**
 * 
 */

UENUM()
enum EGrenadeType
{
	FragGrenade,
	FlashGrenade,
	SmokeGrenade,
	DecoyGrenade,
	IncendiaryGrenade,
	Molotov
};

UCLASS()
class CSTRIKE_API AWeapon_Grenade : public AWeapon_Base
{
	GENERATED_BODY()

public:

	AWeapon_Grenade();

	// Called when player click primary fire button
	virtual void WeaponPrimaryFire() override;

	// Called when player click secondary fire button
	virtual void WeaponSecondaryFire() override;

	// Called when player release fire button
	virtual void WeaponStopFire() override;

	UPROPERTY(EditAnywhere, Category = "Config")
	TEnumAsByte<EGrenadeType> GrenadeType;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AWeapon_GrenadeProjectile> ProjectileClass;

	int32 GrenadeCount;
};

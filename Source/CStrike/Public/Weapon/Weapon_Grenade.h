// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon_Base.h"
#include "Weapon_Grenade.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;
/**
 * 
 */
UCLASS()
class CSTRIKE_API AWeapon_Grenade : public AWeapon_Base
{
	GENERATED_BODY()

public:

	AWeapon_Grenade();

	UPROPERTY(EditAnywhere, Category = "Proejctile")
	USphereComponent* DamageSphere;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UParticleSystemComponent* GrenadeFX;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UProjectileMovementComponent* GrenadeMovementComponent;

	virtual void WeaponPrimaryFire() override;

	virtual void WeaponStopFire() override;
};

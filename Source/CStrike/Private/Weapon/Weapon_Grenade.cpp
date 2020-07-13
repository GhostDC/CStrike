// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon_Grenade.h"
#include "Player/CSPlayer.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Weapon/Weapon_GrenadeProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AWeapon_Grenade::AWeapon_Grenade()
{

}

void AWeapon_Grenade::WeaponPrimaryFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Throw higher Grenade"));
}

void AWeapon_Grenade::WeaponSecondaryFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Throw lower Grenade"));
}

void AWeapon_Grenade::WeaponStopFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Thrown Grenade"));
}
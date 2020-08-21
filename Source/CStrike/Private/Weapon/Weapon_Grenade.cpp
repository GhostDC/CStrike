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
	GrenadeCount = 1;
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
	UE_LOG(LogTemp, Warning, TEXT("Thrown Grenade. Left grenade:%d"),GrenadeCount);
	FVector SpawnLoc = GetAimTransform().GetLocation() + WeaponOwner->GetActorForwardVector() * 100.0f;
	FRotator SpawnRot = GetAimTransform().Rotator();
	GetWorld()->SpawnActor<AWeapon_GrenadeProjectile>(ProjectileClass, SpawnLoc, SpawnRot);
	GrenadeCount--;
	if (GrenadeCount <= 0)
	{
		DetachWeaponFromPlayer();
		WeaponOwner->Mesh1P->PlayAnimation(nullptr, false);
		WeaponOwner->GetMesh()->PlayAnimation(nullptr, false);
		WeaponOwner->CurrentWeapon = nullptr;
		Destroy();
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon_Grenade.h"
#include "Player/CSPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AWeapon_Grenade::AWeapon_Grenade()
{
	DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DamageSphere"));
	RootComponent = DamageSphere;

	GrenadeFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("GrenadeFX"));
	GrenadeFX->bAutoActivate = false;
	GrenadeFX->SetupAttachment(RootComponent);


	GrenadeMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("GrenadeMovementComponent"));
	GrenadeMovementComponent->UpdatedComponent = DamageSphere;
	GrenadeMovementComponent->InitialSpeed = 500.0f;
	GrenadeMovementComponent->MaxSpeed = 500.0f;
	GrenadeMovementComponent->bRotationFollowsVelocity = true;
	GrenadeMovementComponent->bSimulationEnabled = false;
}

void AWeapon_Grenade::WeaponPrimaryFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Throw Grenade"));
}

void AWeapon_Grenade::WeaponStopFire()
{
		UE_LOG(LogTemp, Warning, TEXT("Thrown Grenade"));
}
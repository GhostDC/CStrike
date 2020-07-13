// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon_GrenadeProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AWeapon_GrenadeProjectile::AWeapon_GrenadeProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = CollisionComp;

	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeMesh"));
	GrenadeMesh->SetupAttachment(RootComponent);

	GrenadeFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("GrenadeFX"));
	GrenadeFX->SetupAttachment(RootComponent);

	GrenadeProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("GrenadeProjectile"));
	GrenadeProjectile->InitialSpeed = 3000.0f;
	GrenadeProjectile->MaxSpeed = 3000.0f;
	GrenadeProjectile->bRotationFollowsVelocity = true;
}

// Called when the game starts or when spawned
void AWeapon_GrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon_GrenadeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


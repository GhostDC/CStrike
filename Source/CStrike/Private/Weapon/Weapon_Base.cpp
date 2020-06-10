// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Weapon_Base.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AWeapon_Base::AWeapon_Base()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set weapon first mesh
	WeaponMesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh1P"));
	WeaponMesh1P->SetCastShadow(false);
	RootComponent = WeaponMesh1P;

	// Set weapon third mesh
	WeaponMesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh3P"));
	WeaponMesh3P->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon_Base::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when weapon primary fire
void AWeapon_Base::WeaponPrimaryFire()
{
	UE_LOG(LogTemp, Log, TEXT("Weapon primary fire"));
}

// Called when weapon secondry fire
void AWeapon_Base::WeaponSecondryFire()
{
	UE_LOG(LogTemp, Log, TEXT("Weapon secondry fire"));
}

// Called when weapon reload
void AWeapon_Base::WeaponReload()
{
	UE_LOG(LogTemp, Log, TEXT("Weapon Reload"));
}

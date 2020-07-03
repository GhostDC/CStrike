// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Weapon_Base.h"
#include "Player/CSPlayer.h"
#include "Weapon/Item_Pickup.h"
#include "Camera/CameraComponent.h"
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

	PrimaryAmmo = WeaponConfig.PrimaryClipSize;
	ReserveAmmo = WeaponConfig.ReserveAmmoMax;
}

// Called every frame
void AWeapon_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when weapon primary fire
void AWeapon_Base::WeaponPrimaryFire()
{
	if (WeaponOwner)
	{
		UE_LOG(LogTemp, Log, TEXT("Weapon primary fire"));
		if (WeaponType == EWeaponType::Knife)
		{
			WeaponMesh1P->PlayAnimation(FireAnimation[0], false);
			WeaponOwner->Mesh1P->PlayAnimation(FireAnimation[1], false);
			UE_LOG(LogTemp, Warning, TEXT("PrimaryAmmo:%d ReserveAmmo:%d"), PrimaryAmmo, ReserveAmmo);
		}
		else if (PrimaryAmmo > 0)
		{
			WeaponMesh1P->PlayAnimation(FireAnimation[0], false);
			WeaponOwner->Mesh1P->PlayAnimation(FireAnimation[1], false);
			PrimaryAmmo--;
			UE_LOG(LogTemp, Warning, TEXT("PrimaryAmmo:%d ReserveAmmo:%d"), PrimaryAmmo, ReserveAmmo);
		}
	}
}

// Called when weapon secondry fire
void AWeapon_Base::WeaponSecondryFire()
{
	UE_LOG(LogTemp, Log, TEXT("Weapon secondry fire"));
}

// Called when weapon reload
void AWeapon_Base::WeaponReload()
{
	if (WeaponOwner && !(WeaponType == EWeaponType::Knife || WeaponType == EWeaponType::C4 || WeaponType == EWeaponType::Grenade))
	{
		UE_LOG(LogTemp, Log, TEXT("Weapon Reload"));
		if (ReserveAmmo > WeaponConfig.PrimaryClipSize)
		{
			ReserveAmmo -= (WeaponConfig.PrimaryClipSize - PrimaryAmmo);
			PrimaryAmmo = WeaponConfig.PrimaryClipSize;
			WeaponMesh1P->PlayAnimation(ReloadAnimation[0], false);
			WeaponOwner->Mesh1P->PlayAnimation(ReloadAnimation[1], false);
			UE_LOG(LogTemp, Warning, TEXT("PrimaryAmmo:%d ReserveAmmo:%d"), PrimaryAmmo, ReserveAmmo);
		}
		else if (ReserveAmmo < WeaponConfig.PrimaryClipSize && ReserveAmmo > 0)
		{
			PrimaryAmmo = ReserveAmmo;
			ReserveAmmo = 0;
			WeaponMesh1P->PlayAnimation(ReloadAnimation[0], false);
			WeaponOwner->Mesh1P->PlayAnimation(ReloadAnimation[1], false);
			UE_LOG(LogTemp, Warning, TEXT("PrimaryAmmo:%d ReserveAmmo:%d"), PrimaryAmmo, ReserveAmmo);
		}
		else if (PrimaryAmmo == WeaponConfig.PrimaryClipSize)
		{
			UE_LOG(LogTemp, Warning, TEXT("No need to reload"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No ammo can be reload"));
		}
	}
}

// Called when player equip this weapon
void AWeapon_Base::WeaponDraw(ACSPlayer *DrawPlayer)
{
	if (DrawPlayer)
	{
		WeaponOwner = DrawPlayer;
		this->AttachToComponent(WeaponOwner->Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
		WeaponOwner->CurrentWeapon = this;
		WeaponMesh1P->PlayAnimation(DrawAnimation[0], false);
		WeaponOwner->Mesh1P->PlayAnimation(DrawAnimation[1], false);
	}
}

// Called player drop weapon and spawn a pick up blueprint for weapon
void AWeapon_Base::WeaponDrop()
{
	int32 WeaponIndex;
	if (WeaponOwner->WeaponSlot.Find(this, WeaponIndex))
	{
		AItem_Pickup *PickupClass = GetWorld()->SpawnActor<AItem_Pickup>(PickupBlueprint, GetActorTransform());
		WeaponOwner->WeaponSlot[WeaponIndex] = nullptr;
		PickupClass->ItemMesh->AddImpulse(WeaponOwner->FPSCamera->GetForwardVector() * 300, NAME_None, true);
		PickupClass->PrimaryAmmo = PrimaryAmmo;
		PickupClass->ReserveAmmo = ReserveAmmo;
		WeaponOwner->CurrentWeapon = nullptr;
		WeaponOwner->Mesh1P->PlayAnimation(nullptr, false);
		Destroy();
	}
}

// Called player want to inspect weapon
void AWeapon_Base::WeaponInspect()
{
	WeaponMesh1P->PlayAnimation(InspectAnimation[0], false);
	WeaponOwner->Mesh1P->PlayAnimation(InspectAnimation[1], false);
}
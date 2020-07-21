// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon_C4.h"

// Called when player start plant c4
void AWeapon_C4::WeaponPrimaryFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Start plant C4"));
}

// Called when player stop plant
void AWeapon_C4::WeaponStopFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop plant C4"));
}

// Called C4 plant finished
void AWeapon_C4::Planted()
{
	UE_LOG(LogTemp, Warning, TEXT("C4 planted"));
}

// Called when C4 time is up
void AWeapon_C4::Explode()
{
	UE_LOG(LogTemp, Warning, TEXT("C4 explode"));
}
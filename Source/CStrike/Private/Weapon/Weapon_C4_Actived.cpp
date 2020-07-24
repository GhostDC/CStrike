// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon_C4_Actived.h"

void AWeapon_C4_Actived::BeginPlay()
{
	Super::BeginPlay();
	WeaponDropMesh->SetGenerateOverlapEvents(false);
	GetWorldTimerManager().SetTimer(ExplodeTimer, this, &AWeapon_C4_Actived::Explode, C4Config.ExplodeTime, false);
}

void AWeapon_C4_Actived::Explode()
{
	UE_LOG(LogTemp, Warning, TEXT("C4 explode"));
}
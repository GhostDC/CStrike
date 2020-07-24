// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon_C4.h"
#include "Player/CSPlayer.h"
#include "Weapon/Weapon_C4_Actived.h"

// Called every frame
void AWeapon_C4::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (WeaponOwner)
	{
		if (!WeaponOwner->GetVelocity().IsZero() && IsPlanting)
		{
			WeaponStopFire();
		}
	}
}

// Called when player start plant c4
void AWeapon_C4::WeaponPrimaryFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Start plant C4"));
	IsPlanting = true;
	WeaponMesh1P->PlayAnimation(FireAnimation[0], false);
	WeaponOwner->Mesh1P->PlayAnimation(FireAnimation[1], false);
	UAnimSequence* FireSeq = Cast<UAnimSequence>(FireAnimation[0]);
	if (FireSeq)
	{
		float FireDuration = FireSeq->GetPlayLength();
		GetWorldTimerManager().SetTimer(PlantTimer, this, &AWeapon_C4::Planted, FireDuration, false);
	}
}

// Called when player stop plant
void AWeapon_C4::WeaponStopFire()
{
	if (IsPlanting)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stop plant C4"));
		IsPlanting = false;
	}
}

// Called C4 plant finished
void AWeapon_C4::Planted()
{
	if (IsPlanting)
	{
		UE_LOG(LogTemp, Warning, TEXT("C4 planted"));
		FVector SpawnLoc = WeaponOwner->GetActorLocation();
		FRotator SpawnRot = GetAimTransform().Rotator();
		WeaponMesh1P->PlayAnimation(nullptr, false);
		WeaponOwner->Mesh1P->PlayAnimation(nullptr, false);
		GetWorld()->SpawnActor<AWeapon_C4_Actived>(PlantedC4, SpawnLoc, SpawnRot);
		WeaponOwner = nullptr;
		DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		DetachWeaponFromPlayer();
		Destroy();
	}
}
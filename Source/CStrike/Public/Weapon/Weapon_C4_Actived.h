// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon_C4.h"
#include "Weapon_C4_Actived.generated.h"

/**
 * 
 */
UCLASS()
class CSTRIKE_API AWeapon_C4_Actived : public AWeapon_C4
{
	GENERATED_BODY()

	UFUNCTION()
	virtual void BeginPlay() override;

	FTimerHandle ExplodeTimer;

	UFUNCTION()
	void Explode();
};

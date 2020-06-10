// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon_Base.generated.h"

class USkeletalMeshComponent;

UCLASS()
class CSTRIKE_API AWeapon_Base : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon_Base();

	// Set weapon first person mesh
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USkeletalMeshComponent *WeaponMesh1P;

	// Set weapon third person mesh
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USkeletalMeshComponent *WeaponMesh3P;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when weapon primary fire
	UFUNCTION()
	void WeaponPrimaryFire();

	// Called when weapon secondry fire
	UFUNCTION()
	void WeaponSecondryFire();

	// Called when weapon reload
	UFUNCTION()
	void WeaponReload();
};

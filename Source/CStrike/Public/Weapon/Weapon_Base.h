// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon_Base.generated.h"

class ACSPlayer;
class AItem_Pickup;
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

	// Set weapon pick up blueprint after drop
	UPROPERTY(EditAnywhere, Category = "Pickup")
	TSubclassOf<AItem_Pickup> PickupBlueprint;

	// Weapon owner
	UPROPERTY(EditAnywhere, Category = "Owner")
	ACSPlayer *WeaponOwner;

	// Set weapon draw animation
	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<UAnimationAsset *> DrawAnimation;

	// Set hand draw this weapon animation
	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<UAnimationAsset *> HandDrawAnimation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when weapon primary fire
	UFUNCTION(BlueprintCallable, Category = "Action")
	void WeaponPrimaryFire();

	// Called when weapon secondry fire
	UFUNCTION(BlueprintCallable, Category = "Action")
	void WeaponSecondryFire();

	// Called when weapon reload
	UFUNCTION(BlueprintCallable, Category = "Action")
	void WeaponReload();

	// Called when weapon equip
	UFUNCTION(BlueprintCallable, Category = "Action")
	void WeaponDraw(ACSPlayer *DrawPlayer);

	// Called when weapon drop
	UFUNCTION(BlueprintCallable, Category = "Action")
	void WeaponDrop();
};

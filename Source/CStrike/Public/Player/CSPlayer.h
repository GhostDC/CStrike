// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CSPlayer.generated.h"

class AWeapon_Base;
class UCameraComponent;
class USkeletalMeshComponent;

UCLASS()
class CSTRIKE_API ACSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACSPlayer();

	// Set first player mesh
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USkeletalMeshComponent *Mesh1P;

	// Set first player camera
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent *FPSCamera;

	// Set player weapon slot
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapon_Base *CurrentWeapon;

	// Set ray trace range
	UPROPERTY(EditAnywhere, Category = "Config")
	float TraceRange = 3000.0f;

	// Weapon slot
	UPROPERTY(EditAnywhere, Category = "Config")
	TArray<AWeapon_Base *> WeaponSlot;

	// Setup player health
	float Health = 100.0f;

	// Setup player ammor
	float Ammor = 0.0f;

	// Auto swich new weapon
	UPROPERTY()
	bool IsSwitchToNewWeapon = true;

	// Catch ray trace hit result
	UPROPERTY()
	FHitResult HitResult;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to ray trace per tick
	UFUNCTION()
	void RayTracePerTick();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	//////////////////////////////////////////////////////
	// Movement
	// Called when player wants to move forward
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float Val);

	// Called when player wants to move right
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveRight(float Val);

	//////////////////////////////////////////////////////
	// Weapon action
	// Called when player wants to primary fire
	UFUNCTION(BlueprintCallable, Category = "Action")
	void PlayerFirePrimary();

	// Called when player wants to secondary fire
	UFUNCTION(BlueprintCallable, Category = "Action")
	void PlayerFireSecondary();

	// Called when player wants to reload or when weapon ammo empty and primary fire release
	UFUNCTION(BlueprintCallable, Category = "Action")
	void PlayerReload();

	// Called when player wants to drop item
	UFUNCTION(BlueprintCallable, Category = "Action")
	void PlayerDrop();

	// Called when player wants to inspect weapon
	UFUNCTION(BlueprintCallable, Category = "Action")
	void PlayerInspect();

	// Called when weapon fire hit player
	//UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};

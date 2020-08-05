// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon_Base.generated.h"

class ACSPlayer;
class AItem_Pickup;
class UAnimationAsset;
class UStaticMeshComponent;
class USkeletalMeshComponent;

// Setup weapon data
USTRUCT()
struct FWeaponData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Config")
	bool isFullAuto;

	UPROPERTY(EditAnywhere, Category = "Config")
	bool isBurstFire;

	UPROPERTY(EditAnywhere, Category = "Config")
	int32 BurstShotCost;

	UPROPERTY(EditAnywhere, Category = "Config")
	int32 ZoomLevel;

	UPROPERTY(EditAnywhere, Category = "Config")
	float BaseDamage;

	UPROPERTY(EditAnywhere, Category = "Config")
	float Penetrate;

	UPROPERTY(EditAnywhere, Category = "Config")
	int32 PrimaryClipSize;

	UPROPERTY(EditAnywhere, Category = "Config")
	int32 ReserveAmmoMax;

	UPROPERTY(EditAnywhere, Category = "Config")
	int32 WeaponPrice;

	UPROPERTY(EditAnywhere, Category = "Config")
	float ShotRange;

	UPROPERTY(EditAnywhere, Category = "Config")
	float CycleTime;

	UPROPERTY(EditAnywhere, Category = "Config")
	float Weight;

	UPROPERTY(EditAnywhere, Category = "Config")
	FName WeaponName;
};

// Setup weapon type
UENUM()
enum EWeaponType
{
	Knife,
	Pistol,
	SubMachineGun,
	ShotGun,
	MachineGun,
	Rifle,
	SniperRifle,
	C4,
	Grenade
};

UCLASS()
class CSTRIKE_API AWeapon_Base : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon_Base();

	// Set weapon drop mesh
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* WeaponDropMesh;

	// Set weapon first person mesh
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USkeletalMeshComponent *WeaponMesh1P;

	// Set weapon third person mesh
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USkeletalMeshComponent *WeaponMesh3P;

	// Weapon owner
	UPROPERTY(EditAnywhere, Category = "Owner")
	ACSPlayer *WeaponOwner;

	// Weapon type
	UPROPERTY(EditAnywhere, Category = "Config")
	TEnumAsByte<EWeaponType> WeaponType;

	// Weapon data
	UPROPERTY(EditAnywhere, Category = "Config")
	FWeaponData WeaponConfig;

	// Set weapon fire animation
	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<UAnimationAsset *> FireAnimation;

	// Set weapon reload animation
	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<UAnimationAsset *> ReloadAnimation;

	// Set weapon draw animation
	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<UAnimationAsset *> DrawAnimation;

	// Set Weapon draw animation
	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<UAnimationAsset *> InspectAnimation;

	TArray<FHitResult> HitResults;

	// WeaponAmmo
	int32 PrimaryAmmo = -1;

	int32 ReserveAmmo = -1;

	// ZoomTime
	int32 ZoomTime;

	// BurstTime
	int32 BurstTime;

	// Weapon spawn time
	int32 SpawnTime;

	FTimerHandle ReloadTimer;

	FTimerHandle ClipInTimer;

	FTimerHandle FireTimer;

	FTimerHandle BurstTimer;

protected:
	// Called when the game starts or when spawned
	virtual void
	BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when player overlap
	UFUNCTION()
	virtual void WeaponOverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when weapon primary fire
	UFUNCTION(BlueprintCallable, Category = "Action")
	virtual void WeaponPrimaryFire();

	// Called when weapon secondry fire
	UFUNCTION(BlueprintCallable, Category = "Action")
	virtual void WeaponSecondaryFire();

	// Called when player release fire
	UFUNCTION(BlueprintCallable, Category = "Action")
	virtual void WeaponStopFire();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void WeaponInstantFire();

	// Called when weapon reload
	UFUNCTION(BlueprintCallable, Category = "Action")
	void WeaponReload();

	// Called when weapon equip
	UFUNCTION(BlueprintCallable, Category = "Action")
	void WeaponDraw(ACSPlayer *DrawPlayer);

	// Called when player get weapon
	UFUNCTION()
	void WeaponAddToPlayer(ACSPlayer* DrawPlayer);

	// Called when weapon drop
	UFUNCTION(BlueprintCallable, Category = "Action")
	void WeaponDrop();

	// Called when weapon need to play animation
	UFUNCTION()
	void WeaponPlayAnimation(TArray<UAnimationAsset*> AnimationArray,int32 WeaponAnimIndex, int32 PlayerAnimIndex);

	// Called when need to attach weapon to player
	UFUNCTION()
	void AttachWeaponToPlayer();

	// Called when need to detach weapon from player
	UFUNCTION()
	void DetachWeaponFromPlayer();

	// Called when player want inspect weapon
	UFUNCTION(BlueprintCallable, Category = "Action")
	void WeaponInspect();

	// Called when weapon per shot
	UFUNCTION(BlueprintCallable, Category = "Action")
	void WeaponTracePerShot();

	// Get player aim transform
	FTransform GetAimTransform();

	// Check weapon can fire or not before weapon fire
	bool CanFire();

	// Check weapon can reload or not before weapon reload
	bool CanReload();
};

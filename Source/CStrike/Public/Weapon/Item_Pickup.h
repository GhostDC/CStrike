// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item_Pickup.generated.h"

class AWeapon_Base;
class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class CSTRIKE_API AItem_Pickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem_Pickup();

	// Set pick up item collision
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	UBoxComponent *ItemCollisionBox;

	// set pick up item mesh
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent *ItemMesh;

	// Set player pick up blueprint
	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<AWeapon_Base> ItemBlueprint;

	// Set actor spawn time
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 SpawnTime;

	// Drop weapon ammo primary
	UPROPERTY(EditAnywhere, Category = "Config")
	int32 PrimaryAmmo;

	// Drop weapon ammo reserve
	UPROPERTY(EditAnywhere, Category = "Config")
	int32 ReserveAmmo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when player overlap this actor
	UFUNCTION()
	void OverlapHandler(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};

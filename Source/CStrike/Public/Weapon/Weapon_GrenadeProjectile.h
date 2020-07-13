// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon_GrenadeProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleMovementComponent;
class UProjectileMovementComponent;

UCLASS()
class CSTRIKE_API AWeapon_GrenadeProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon_GrenadeProjectile();

	UPROPERTY(EditAnywhere, Category = "Projectile")
	USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UStaticMeshComponent* GrenadeMesh;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UParticleSystemComponent* GrenadeFX;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UProjectileMovementComponent* GrenadeProjectile;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

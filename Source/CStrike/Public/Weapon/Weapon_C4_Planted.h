// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon_C4_Planted.generated.h"

UCLASS()
class CSTRIKE_API AWeapon_C4_Planted : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon_C4_Planted();

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* PlantedMesh;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* ExplodeSound;

	FTimerHandle ExplodeTimer;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Planted();

	UFUNCTION()
	void Explode();
};

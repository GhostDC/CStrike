// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CSPlayer.generated.h"

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

	// Catch ray trace hit result
	UPROPERTY()
	FHitResult HitResult;

	// Set ray trace range
	UPROPERTY(EditAnywhere, Category = "Config")
	float TraceRange = 3000.0f;

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

	// Called when player wants to move forward
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float Val);

	// Called when player wants to move right
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveRight(float Val);
};

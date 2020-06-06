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

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USkeletalMeshComponent *Mesh1P;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent *FPSCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float Val);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveRight(float Val);
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/CSPlayer.h"

// Sets default values
ACSPlayer::ACSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACSPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACSPlayer::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

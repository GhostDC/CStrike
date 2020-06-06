// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/CSPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACSPlayer::ACSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->bUsePawnControlRotation = true;
	FPSCamera->SetupAttachment(RootComponent);

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->bOnlyOwnerSee = true;
	Mesh1P->SetCastShadow(false);
	Mesh1P->SetupAttachment(FPSCamera);

	GetMesh()->bOwnerNoSee = true;
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
	RayTracePerTick();
}

// Called to bind functionality to input
void ACSPlayer::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACSPlayer::MoveForward(float Val)
{
	if (Val != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void ACSPlayer::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Val);
	}
}

void ACSPlayer::RayTracePerTick()
{
	FVector TraceStart = FPSCamera->GetComponentLocation();
	FVector TraceEnd = TraceStart + (FPSCamera->GetForwardVector() * TraceRange);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Orange);
	if (HitResult.bBlockingHit)
	{
		DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 15.0f, FColor::Orange);
	}
}
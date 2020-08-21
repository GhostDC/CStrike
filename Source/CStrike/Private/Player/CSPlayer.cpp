// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/CSPlayer.h"
#include "Weapon/Weapon_Base.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACSPlayer::ACSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set first player Camera
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->bUsePawnControlRotation = true;
	FPSCamera->SetupAttachment(RootComponent);

	// Set first player mesh
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->bOnlyOwnerSee = true;
	Mesh1P->SetCastShadow(false);
	Mesh1P->SetupAttachment(FPSCamera);

	// Set player can not see third mesh
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

// Called to ray trace per tick
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

//////////////////////////////////////////////////////
// Movement
// Called when player move forward
void ACSPlayer::MoveForward(float Val)
{
	if (Val != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

// Called when player move right
void ACSPlayer::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Val);
	}
}

//////////////////////////////////////////////////////
// Weapon action
// Called when player wants to primary fire
void ACSPlayer::PlayerFirePrimary()
{
	if (CurrentWeapon)
		CurrentWeapon->WeaponPrimaryFire();
}

// Called when player want to secondary fire
void ACSPlayer::PlayerFireSecondary()
{
	if (CurrentWeapon)
		CurrentWeapon->WeaponSecondaryFire();
}

void ACSPlayer::PlayerStopFire()
{
	if (CurrentWeapon)
		CurrentWeapon->WeaponStopFire();
}

// Called when player wants to reload
void ACSPlayer::PlayerReload()
{
	if (CurrentWeapon)
		CurrentWeapon->WeaponReload();
}

// Called when player want to drop item
void ACSPlayer::PlayerDrop()
{
	if (CurrentWeapon)
		CurrentWeapon->WeaponDrop();
	if (LastWeapon)
		LastWeapon->WeaponDraw(this);
}

// Called when player want to inpect weapon
void ACSPlayer::PlayerInspect()
{
	if (CurrentWeapon)
		CurrentWeapon->WeaponInspect();
}

// Called when player want to pickup weapon
void ACSPlayer::PlayerPickup()
{
	if (HitResult.bBlockingHit && HitResult.Distance < PickupRange)
	{
		AWeapon_Base* Weapon = Cast<AWeapon_Base>(HitResult.Actor);
		if (Weapon)
		{
			Weapon->WeaponDraw(this);
		}
	}
}

float ACSPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("TakeDamage : %d"), DamageAmount);
	if (Ammor == 0)
	{
		Health -= DamageAmount;
		if (Health <= 0.0f)
		{
			this->Destroy();
		}
	}
	return Health;
}
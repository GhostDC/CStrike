// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Weapon_Base.h"
#include "Player/CSPlayer.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AWeapon_Base::AWeapon_Base()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponDropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponDropMesh"));
	WeaponDropMesh->SetSimulatePhysics(true);
	WeaponDropMesh->SetGenerateOverlapEvents(false);
	RootComponent = WeaponDropMesh;

	// Set weapon first mesh
	WeaponMesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh1P"));
	WeaponMesh1P->CastShadow = false;
	WeaponMesh1P->bOnlyOwnerSee = true;
	WeaponMesh1P->SetupAttachment(RootComponent);

	// Set weapon third mesh
	WeaponMesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh3P"));
	WeaponMesh3P->bOwnerNoSee = true;
	WeaponMesh3P->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon_Base::BeginPlay()
{
	Super::BeginPlay();
	WeaponDropMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeapon_Base::WeaponOverlapHandler);
	SpawnTime = FPlatformTime::Seconds();
	if (PrimaryAmmo == -1 && ReserveAmmo == -1)
	{
		PrimaryAmmo = WeaponConfig.PrimaryClipSize;
		ReserveAmmo = WeaponConfig.ReserveAmmoMax;
	}
}

// Called every frame
void AWeapon_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FPlatformTime::Seconds() - SpawnTime >= 1.0f)
	{
		WeaponDropMesh->SetGenerateOverlapEvents(true);
	}
}

void AWeapon_Base::WeaponOverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ACSPlayer>())
	{
		ACSPlayer* OverlapActor = Cast<ACSPlayer>(OtherActor);
		if (OverlapActor)
		{
			WeaponDraw(OverlapActor);
		}
	}
}

// Called when weapon primary fire
void AWeapon_Base::WeaponPrimaryFire()
{
	if (CanFire())
	{
		UE_LOG(LogTemp, Log, TEXT("Weapon primary fire"));
		if (WeaponType == EWeaponType::Knife)
		{
			WeaponMesh1P->PlayAnimation(FireAnimation[0], false);
			WeaponOwner->Mesh1P->PlayAnimation(FireAnimation[1], false);
			UE_LOG(LogTemp, Warning, TEXT("PrimaryAmmo:%d ReserveAmmo:%d"), PrimaryAmmo, ReserveAmmo);
		}
		else if (PrimaryAmmo > 0)
		{
			if (WeaponConfig.isFullAuto)
			{
				GetWorldTimerManager().SetTimer(FireTimer, this, &AWeapon_Base::WeaponInstantFire, WeaponConfig.CycleTime, true, 0.0f);
			}
			else
			{
				if (WeaponConfig.isBrustFire)
				{
					for (int32 i = 0; i < WeaponConfig.BrustShotCost; i++)
					{
						GetWorldTimerManager().SetTimer(FireTimer, this, &AWeapon_Base::WeaponInstantFire, WeaponConfig.CycleTime, false, 0.0f);
					}
				}
				else WeaponInstantFire();
			}
		}
	}
}

// Called when weapon secondry fire
void AWeapon_Base::WeaponSecondryFire()
{
	if (WeaponConfig.WeaponName == TEXT("glock18") || WeaponConfig.WeaponName == TEXT("famas"))
	{
		WeaponConfig.isBrustFire = true;
	}
	else if (WeaponType == EWeaponType::SniperRifle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Your sniper rifle is scoped"));
	}
}

// Called when player release fire
void AWeapon_Base::WeaponStopFire()
{
	if (GetWorldTimerManager().IsTimerActive(FireTimer))
	{
		GetWorldTimerManager().ClearTimer(FireTimer);
	}
}

// Called when player want to fire weapon multiple times
void AWeapon_Base::WeaponInstantFire()
{
	WeaponMesh1P->PlayAnimation(FireAnimation[0], false);
	WeaponOwner->Mesh1P->PlayAnimation(FireAnimation[1], false);
	if (PrimaryAmmo > 0)
	{
		PrimaryAmmo--;
		WeaponTracePerShot();
		if (HitResults.Num() > 0)
		{
			for (int32 i = 0; i < HitResults.Num(); i++)
			{
				UE_LOG(LogTemp, Warning, TEXT("you just hit : %s"), *HitResults[i].GetActor()->GetFullName());
				if (HitResults[i].Actor->IsA<ACSPlayer>())
				{
					ACSPlayer* HitPlayer = Cast<ACSPlayer>(HitResults[i].Actor);
					TSubclassOf<UDamageType> DamageType;
					UGameplayStatics::ApplyPointDamage(HitPlayer, WeaponConfig.BaseDamage, GetActorLocation(), HitResults[i], WeaponOwner->GetController(), this, DamageType);
					UE_LOG(LogTemp, Warning, TEXT("%s got shot health : %f"), *HitPlayer->GetFullName(), HitPlayer->Health);
				}
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("PrimaryAmmo:%d ReserveAmmo:%d"), PrimaryAmmo, ReserveAmmo);
	}
}

// Called when weapon reload
void AWeapon_Base::WeaponReload()
{
	if (CanReload() && !(WeaponType == EWeaponType::Knife || WeaponType == EWeaponType::C4 || WeaponType == EWeaponType::Grenade))
	{
		UE_LOG(LogTemp, Log, TEXT("Weapon Reload"));
		if (ReserveAmmo > WeaponConfig.PrimaryClipSize)
		{
			ReserveAmmo -= (WeaponConfig.PrimaryClipSize - PrimaryAmmo);
			PrimaryAmmo = WeaponConfig.PrimaryClipSize;
			WeaponMesh1P->PlayAnimation(ReloadAnimation[0], false);
			WeaponOwner->Mesh1P->PlayAnimation(ReloadAnimation[1], false);
			UE_LOG(LogTemp, Warning, TEXT("PrimaryAmmo:%d ReserveAmmo:%d"), PrimaryAmmo, ReserveAmmo);
		}
		else if (ReserveAmmo < WeaponConfig.PrimaryClipSize && ReserveAmmo > 0)
		{
			if (WeaponConfig.PrimaryClipSize - PrimaryAmmo > 0)
			{
				ReserveAmmo -= (WeaponConfig.PrimaryClipSize - PrimaryAmmo);
				PrimaryAmmo = WeaponConfig.PrimaryClipSize;
				WeaponMesh1P->PlayAnimation(ReloadAnimation[0], false);
				WeaponOwner->Mesh1P->PlayAnimation(ReloadAnimation[1], false);
				UE_LOG(LogTemp, Warning, TEXT("PrimaryAmmo:%d ReserveAmmo:%d"), PrimaryAmmo, ReserveAmmo);
			}
			else
			{
				PrimaryAmmo = ReserveAmmo;
				ReserveAmmo = 0;
				WeaponMesh1P->PlayAnimation(ReloadAnimation[0], false);
				WeaponOwner->Mesh1P->PlayAnimation(ReloadAnimation[1], false);
				UE_LOG(LogTemp, Warning, TEXT("PrimaryAmmo:%d ReserveAmmo:%d"), PrimaryAmmo, ReserveAmmo);
			}
		}
		else if (PrimaryAmmo == WeaponConfig.PrimaryClipSize)
		{
			UE_LOG(LogTemp, Warning, TEXT("No need to reload"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No ammo can be reload"));
		}
	}
}

// Called when player equip this weapon
void AWeapon_Base::WeaponDraw(ACSPlayer* DrawPlayer)
{
	if (DrawPlayer)
	{
		if (WeaponType == EWeaponType::Pistol && !DrawPlayer->WeaponSlot[1])
		{
			SetOwner(DrawPlayer);
			WeaponOwner = DrawPlayer;
			WeaponDropMesh->SetVisibility(false);
			WeaponDropMesh->SetSimulatePhysics(false);
			WeaponDropMesh->SetGenerateOverlapEvents(false);
			AttachToActor(WeaponOwner, FAttachmentTransformRules::SnapToTargetIncludingScale);
			AttachWeaponToPlayer();
			WeaponOwner->CurrentWeapon = this;
			WeaponOwner->WeaponSlot[1] = this;
			WeaponMesh1P->PlayAnimation(DrawAnimation[0], false);
			WeaponOwner->Mesh1P->PlayAnimation(DrawAnimation[1], false);
		}
	}
}

// Called player drop weapon and spawn a pick up blueprint for weapon
void AWeapon_Base::WeaponDrop()
{
	int32 WeaponIndex;
	if (WeaponOwner->WeaponSlot.Find(this, WeaponIndex))
	{
		WeaponDropMesh->SetGenerateOverlapEvents(false);
		SpawnTime = FPlatformTime::Seconds();
		this->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		DetachWeaponFromPlayer();
		WeaponDropMesh->SetSimulatePhysics(true);
		WeaponDropMesh->SetVisibility(true);
		WeaponOwner->WeaponSlot[WeaponIndex] = nullptr;
		WeaponOwner->CurrentWeapon = nullptr;
		WeaponOwner->Mesh1P->PlayAnimation(nullptr, false);
		WeaponDropMesh->AddImpulse(WeaponOwner->FPSCamera->GetForwardVector() * 500.0f, NAME_None, true);
		WeaponOwner = nullptr;
		this->SetOwner(nullptr);
	}
}

// Called when need to attach weapon to player
void AWeapon_Base::AttachWeaponToPlayer()
{
	if (WeaponOwner)
	{
		FName AttachPoint = WeaponOwner->AttachSocket;
		WeaponMesh1P->AttachToComponent(WeaponOwner->Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
		WeaponMesh3P->AttachToComponent(WeaponOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
	}
}

// Called when need to detach weapon from player
void AWeapon_Base::DetachWeaponFromPlayer()
{
	WeaponMesh1P->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	WeaponMesh3P->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
}

// Called player want to inspect weapon
void AWeapon_Base::WeaponInspect()
{
	WeaponMesh1P->PlayAnimation(InspectAnimation[0], false);
	WeaponOwner->Mesh1P->PlayAnimation(InspectAnimation[1], false);
}

// Called when weapon per shot
void AWeapon_Base::WeaponTracePerShot()
{
	FVector TraceStart = WeaponMesh1P->GetSocketLocation("FlashSocket");
	FVector TraceEnd = WeaponOwner->FPSCamera->GetComponentLocation() + (WeaponOwner->FPSCamera->GetForwardVector() * WeaponConfig.ShotRange);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(WeaponOwner);
	GetWorld()->LineTraceMultiByChannel(HitResults, TraceStart, TraceEnd, ECC_Camera, CollisionQueryParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Orange, true, 2.0f);
}

// Check weapon can fire or not before weapon fire
bool AWeapon_Base::CanFire()
{
	return (WeaponOwner && PrimaryAmmo > 0);
}

// Check weapon can reload or not before weapon reload
bool AWeapon_Base::CanReload()
{
	return (WeaponOwner && PrimaryAmmo != WeaponConfig.PrimaryClipSize && ReserveAmmo > 0);
}
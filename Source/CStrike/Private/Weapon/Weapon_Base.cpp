// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Weapon_Base.h"
#include "Player/CSPlayer.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "Weapon/Weapon_Grenade.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
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
	if (FPlatformTime::Seconds() - SpawnTime >= 1.0f && !GetOwner())
		WeaponDropMesh->SetGenerateOverlapEvents(true);
}

void AWeapon_Base::WeaponOverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ACSPlayer>() || OtherComp)
	{
		ACSPlayer* OverlapActor = Cast<ACSPlayer>(OtherActor);
		ACSPlayer* OverlapCompOwner = Cast<ACSPlayer>(OtherComp->GetOwner());
		if (OverlapActor)
			WeaponDraw(OverlapActor);
		else if (OverlapCompOwner)
			WeaponDraw(OverlapCompOwner);
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
			if (FireAnimation.Num() >= 2)
				WeaponPlayAnimation(FireAnimation, 0, 1);
			UE_LOG(LogTemp, Warning, TEXT("PrimaryAmmo:%d ReserveAmmo:%d"), PrimaryAmmo, ReserveAmmo);
		}
		else if (PrimaryAmmo > 0)
		{
			if (WeaponConfig.isFullAuto)
				GetWorldTimerManager().SetTimer(FireTimer, this, &AWeapon_Base::WeaponInstantFire, WeaponConfig.CycleTime, true, 0.0f);
			else
			{
				if (WeaponConfig.isBurstFire)
				{
					GetWorldTimerManager().SetTimer(BurstTimer, this, &AWeapon_Base::WeaponInstantFire, WeaponConfig.CycleTime, true, 0.0f);
					//WeaponInstantFire();
				}
				else WeaponInstantFire();
			}
		}
	}
}

// Called when weapon secondry fire
void AWeapon_Base::WeaponSecondaryFire()
{
	if (WeaponConfig.WeaponName == TEXT("glock18") || WeaponConfig.WeaponName == TEXT("famas"))
	{
		if (WeaponConfig.isBurstFire)
		{
			BurstTime = 0;
			WeaponConfig.isBurstFire = false;
			UE_LOG(LogTemp, Warning, TEXT("Your %s now in single mode"), *WeaponConfig.WeaponName.ToString());
		}
		else
		{
			BurstTime = 0;
			WeaponConfig.isBurstFire = true;
			UE_LOG(LogTemp, Warning, TEXT("Your %s now in burst mode"), *WeaponConfig.WeaponName.ToString());
		}
	}
	else if (WeaponType == EWeaponType::SniperRifle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Your sniper rifle is scoped"));
		switch (ZoomTime)
		{
		case 0:
			WeaponOwner->FPSCamera->SetFieldOfView(50);
			ZoomTime++;
			break;
		case 1:
			WeaponOwner->FPSCamera->SetFieldOfView(30);
			ZoomTime++;
			break;
		case 2:
			ZoomTime = 0;
			WeaponOwner->FPSCamera->SetFieldOfView(90);
			break;
		default:
			break;
		}
	}

}

// Called when player release fire
void AWeapon_Base::WeaponStopFire()
{
	if (GetWorldTimerManager().IsTimerActive(FireTimer))
		GetWorldTimerManager().ClearTimer(FireTimer);
}

// Called when player want to fire weapon multiple times
void AWeapon_Base::WeaponInstantFire()
{
	if (PrimaryAmmo > 0 && FireAnimation.Num() >= 2)
	{
		WeaponPlayAnimation(FireAnimation, 0, 1);
		PrimaryAmmo--;
		WeaponTracePerShot();
		if (HitResults.Num() > 0)
		{
			for (int32 i = 0; i < HitResults.Num(); i++)
			{
				AActor* HitActor = Cast<AActor>(HitResults[i].GetActor());
				UE_LOG(LogTemp, Warning, TEXT("you just hit : %s"), *HitResults[i].GetActor()->GetFullName());
				if (HitActor->IsA<ACSPlayer>() && HitActor)
				{
					ACSPlayer* HitPlayer = Cast<ACSPlayer>(HitResults[i].GetActor());
					TSubclassOf<UDamageType> DamageType;
					UGameplayStatics::ApplyPointDamage(HitPlayer, WeaponConfig.BaseDamage, GetActorLocation(), HitResults[i], WeaponOwner->GetController(), this, DamageType);
					UE_LOG(LogTemp, Warning, TEXT("%s got shot health : %f"), *HitPlayer->GetFullName(), HitPlayer->Health);
				}
				else if (HitActor->IsA<UStaticMeshComponent>())
				{
					UStaticMeshComponent* HitComponent = Cast<UStaticMeshComponent>(HitResults[i].GetActor());
					HitComponent->AddImpulse(GetActorLocation(), NAME_None, true);
				}
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("PrimaryAmmo:%d ReserveAmmo:%d"), PrimaryAmmo, ReserveAmmo);
		BurstTime++;
		if (WeaponConfig.isBurstFire && BurstTime >= WeaponConfig.BurstShotCost)
		{
			GetWorldTimerManager().ClearTimer(BurstTimer);
			BurstTime = 0;
		}
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
			if (ReloadAnimation.Num() >= 2)
				WeaponPlayAnimation(ReloadAnimation, 0, 1);
			UE_LOG(LogTemp, Warning, TEXT("PrimaryAmmo:%d ReserveAmmo:%d"), PrimaryAmmo, ReserveAmmo);
		}
		else if (ReserveAmmo < WeaponConfig.PrimaryClipSize && ReserveAmmo > 0)
		{
			if (WeaponConfig.PrimaryClipSize - PrimaryAmmo > 0)
			{
				ReserveAmmo -= (WeaponConfig.PrimaryClipSize - PrimaryAmmo);
				PrimaryAmmo = WeaponConfig.PrimaryClipSize;
				if (ReloadAnimation.Num() >= 2)
					WeaponPlayAnimation(ReloadAnimation, 0, 1);
				UE_LOG(LogTemp, Warning, TEXT("PrimaryAmmo:%d ReserveAmmo:%d"), PrimaryAmmo, ReserveAmmo);
			}
			else
			{
				PrimaryAmmo = ReserveAmmo;
				ReserveAmmo = 0;
				if (ReloadAnimation.Num() >= 2)
					WeaponPlayAnimation(ReloadAnimation, 0, 1);
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
		BurstTime = 0;
		if (GetWorldTimerManager().IsTimerActive(BurstTimer))
		{
			GetWorldTimerManager().ClearTimer(BurstTimer);
		}
	}
}

// Called when player equip this weapon
void AWeapon_Base::WeaponDraw(ACSPlayer* DrawPlayer)
{
	if (DrawPlayer)
	{
		if (DrawPlayer->CurrentWeapon)
		{
			DrawPlayer->LastWeapon = DrawPlayer->CurrentWeapon;
			DrawPlayer->CurrentWeapon->DetachWeaponFromPlayer();
			DrawPlayer->CurrentWeapon->Destroy();
			DrawPlayer->CurrentWeapon = nullptr;
		}
		switch (WeaponType)
		{
		case Pistol:
		{
			if (!DrawPlayer->WeaponSlot[1])
			{
				WeaponAddToPlayer(DrawPlayer);
				WeaponOwner->WeaponSlot[1] = this;
			}
		}break;
		case Knife:
		{
			if (!DrawPlayer->WeaponSlot[2])
			{
				WeaponAddToPlayer(DrawPlayer);
				WeaponOwner->WeaponSlot[2] = this;
			}
		}break;
		case Rifle:
		case SniperRifle:
		case ShotGun:
		case MachineGun:
		case SubMachineGun:
		{
			if (!DrawPlayer->WeaponSlot[0])
			{
				WeaponAddToPlayer(DrawPlayer);
				WeaponOwner->WeaponSlot[0] = this;
			}
		}break;
		case Grenade:
		{
			AWeapon_Grenade* PickupGrenade = Cast<AWeapon_Grenade>(this);
			switch (PickupGrenade->GrenadeType)
			{
			case FragGrenade:
				WeaponAddToPlayer(DrawPlayer);
				if (!WeaponOwner->GrenadeSlot[0])
					WeaponOwner->GrenadeSlot[0] = PickupGrenade;
				else
					WeaponOwner->GrenadeSlot[0]->GrenadeCount++;
				break;
			case FlashGrenade:
				WeaponAddToPlayer(DrawPlayer);
				if (!WeaponOwner->GrenadeSlot[1])
					WeaponOwner->GrenadeSlot[1] = PickupGrenade;
				else
					WeaponOwner->GrenadeSlot[1]->GrenadeCount++;
				break;
			case SmokeGrenade:
				WeaponAddToPlayer(DrawPlayer);
				if (!WeaponOwner->GrenadeSlot[2])
					WeaponOwner->GrenadeSlot[2] = PickupGrenade;
				else
					WeaponOwner->GrenadeSlot[2]->GrenadeCount++;
				break;
			case DecoyGrenade:
				WeaponAddToPlayer(DrawPlayer);
				if (!WeaponOwner->GrenadeSlot[3])
					WeaponOwner->GrenadeSlot[3] = PickupGrenade;
				else
					WeaponOwner->GrenadeSlot[3]->GrenadeCount++;
				break;
			case IncendiaryGrenade:
				WeaponAddToPlayer(DrawPlayer);
				if (!WeaponOwner->GrenadeSlot[4])
					WeaponOwner->GrenadeSlot[4] = PickupGrenade;
				else
					WeaponOwner->GrenadeSlot[4]->GrenadeCount++;
				break;
			case Molotov:
				WeaponAddToPlayer(DrawPlayer);
				if (!WeaponOwner->GrenadeSlot[5])
					WeaponOwner->GrenadeSlot[5] = PickupGrenade;
				else
					WeaponOwner->GrenadeSlot[5]->GrenadeCount++;
				break;
			default:
				break;
			}
		}
		case C4:
		{
			if (!DrawPlayer->WeaponSlot[3])
			{
				WeaponAddToPlayer(DrawPlayer);
				WeaponOwner->WeaponSlot[3] = this;
			}
		}
		default:
			break;
		}
		WeaponPlayAnimation(DrawAnimation, 0, 1);
	}
}

void AWeapon_Base::WeaponAddToPlayer(ACSPlayer* DrawPlayer)
{
	SetOwner(DrawPlayer);
	WeaponOwner = DrawPlayer;
	WeaponDropMesh->SetVisibility(false);
	WeaponDropMesh->SetSimulatePhysics(false);
	WeaponDropMesh->SetGenerateOverlapEvents(false);
	AttachToActor(WeaponOwner, FAttachmentTransformRules::SnapToTargetIncludingScale);
	AttachWeaponToPlayer();
	WeaponOwner->CurrentWeapon = this;
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

void AWeapon_Base::WeaponPlayAnimation(TArray<UAnimationAsset*> AnimationArray, int32 WeaponAnimIndex, int32 PlayerAnimIndex)
{
	WeaponMesh1P->PlayAnimation(AnimationArray[WeaponAnimIndex], false);
	WeaponOwner->Mesh1P->PlayAnimation(AnimationArray[PlayerAnimIndex], false);
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
	if (InspectAnimation.Num() >= 2)
	{
		WeaponMesh1P->PlayAnimation(InspectAnimation[0], false);
		WeaponOwner->Mesh1P->PlayAnimation(InspectAnimation[1], false);
	}
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

// Get player aim location transform
FTransform AWeapon_Base::GetAimTransform()
{
	FVector CamLoc;
	FRotator CamRot;
	FTransform CamTrans;
	if (WeaponOwner)
	{
		WeaponOwner->GetActorEyesViewPoint(CamLoc, CamRot);
		CamTrans = FTransform(CamRot, CamLoc);
	}
	return CamTrans;
}

// Get player weapon type
int32 AWeapon_Base::GetWeaponType()
{
	return WeaponType;
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
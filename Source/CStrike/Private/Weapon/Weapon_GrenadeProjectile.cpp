// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon_GrenadeProjectile.h"
#include "Player/CSPlayer.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AWeapon_GrenadeProjectile::AWeapon_GrenadeProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = CollisionComp;

	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeMesh"));
	GrenadeMesh->SetupAttachment(RootComponent);

	GrenadeFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("GrenadeFX"));
	GrenadeFX->SetupAttachment(RootComponent);

	GrenadeProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("GrenadeProjectile"));
	GrenadeProjectile->InitialSpeed = 3000.0f;
	GrenadeProjectile->MaxSpeed = 3000.0f;
	GrenadeProjectile->bRotationFollowsVelocity = true;
	GrenadeProjectile->OnProjectileStop.AddDynamic(this, &AWeapon_GrenadeProjectile::Explode);
}

// Called when the game starts or when spawned
void AWeapon_GrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon_GrenadeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon_GrenadeProjectile::Explode(const FHitResult& OutHit)
{
	UE_LOG(LogTemp, Warning, TEXT("Explode"));
	TArray<FHitResult> OutHits;

	FVector ActorLocation = GetActorLocation();
	FVector Start = ActorLocation;
	FVector End = ActorLocation;

	FCollisionShape ExplodeRange = FCollisionShape::MakeSphere(500.0f);
	DrawDebugSphere(GetWorld(), ActorLocation, ExplodeRange.GetSphereRadius(), 50, FColor::Red, true);

	bool IsHitPhy = GetWorld()->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_WorldStatic, ExplodeRange);
	if (IsHitPhy)
	{
		for (auto& Hit : OutHits)
		{
			UStaticMeshComponent* ImpluseActor = Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent());
			if (ImpluseActor)
			{
				ImpluseActor->AddRadialImpulse(GetActorLocation(), 500.f, 2000.f, ERadialImpulseFalloff::RIF_Constant, true);
			}
		}
	}
	bool IsHitPlayer = GetWorld()->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_Pawn, ExplodeRange);
	if (IsHitPlayer)
	{
		for(auto& Hit:OutHits)
		{
			ACSPlayer* DamagePlayer = Cast<ACSPlayer>(Hit.GetActor());
			if (DamagePlayer)
			{
				float Distance = FVector::Distance(GetActorLocation(),DamagePlayer->GetActorLocation());
				float Damage = 100.0f / Distance * 100.0f;
				TSubclassOf<UDamageType> DamageType;
				UGameplayStatics::ApplyPointDamage(this, Damage, GetActorLocation(), Hit, nullptr, this, DamageType);
			}
		}
	}
}
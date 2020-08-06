// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon_C4_Planted.h"
#include "Player/CSPlayer.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
//#include "Components/StaticMeshComponent.h"

// Sets default values
AWeapon_C4_Planted::AWeapon_C4_Planted()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlantedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlantedMesh"));
	PlantedMesh->SetSimulatePhysics(false);
	PlantedMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	PlantedMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	RootComponent = PlantedMesh;
}

// Called when the game starts or when spawned
void AWeapon_C4_Planted::BeginPlay()
{
	Super::BeginPlay();
	Planted();
}

// Called every frame
void AWeapon_C4_Planted::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon_C4_Planted::Planted()
{
	UE_LOG(LogTemp, Warning, TEXT("Boom has been planted"));
	GetWorldTimerManager().SetTimer(ExplodeTimer, this, &AWeapon_C4_Planted::Explode, 10.0f, false);
}

void AWeapon_C4_Planted::Explode()
{
	UE_LOG(LogTemp, Warning, TEXT("Terrorist win"));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeFX, GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());
	TArray<FHitResult> OutHits;

	FVector ActorLocation = GetActorLocation();

	FCollisionShape ExplodeRange = FCollisionShape::MakeSphere(1200.0f);
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplodeRange.GetSphereRadius(), 50, FColor::Orange, true);
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, ActorLocation, ActorLocation, FQuat::Identity, ECC_WorldStatic, ExplodeRange);
	if (isHit)
	{
		for (auto& Hit : OutHits)
		{
			UStaticMeshComponent* ImpulseActor = Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent());
			if (ImpulseActor)
			{
				ImpulseActor->AddRadialImpulse(ActorLocation, 900.0f, 2000.0f, ERadialImpulseFalloff::RIF_Constant, true);
			}
		}
	}
	bool IsHitPlayer = GetWorld()->SweepMultiByChannel(OutHits, ActorLocation, ActorLocation, FQuat::Identity, ECC_Pawn, ExplodeRange);
	if (IsHitPlayer)
	{
		for (auto& Hit : OutHits)
		{
			ACSPlayer* DamagePlayer = Cast<ACSPlayer>(Hit.GetActor());
			if (DamagePlayer)
			{
				float Distance = FVector::Distance(GetActorLocation(), DamagePlayer->GetActorLocation());
				UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);
				float Damage = 300.0f / Distance * 100.0f;
				TSubclassOf<UDamageType> DamageType;
				TArray<AActor*> IgnoreActor;
				UGameplayStatics::ApplyRadialDamage(this, 300.0f, ActorLocation, 900.0f, DamageType, IgnoreActor, this, nullptr, false, ECC_Pawn);
			}
		}
	}
}
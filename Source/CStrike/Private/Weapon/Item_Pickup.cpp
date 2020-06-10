// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Item_Pickup.h"
#include "Player/CSPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AItem_Pickup::AItem_Pickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set pick up item mesh
	ItemCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("IteamCollisionBox"));
	ItemCollisionBox->SetGenerateOverlapEvents(true);
	ItemCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AItem_Pickup::OverlapHandler);
	RootComponent = ItemCollisionBox;

	// Set pick up item mesh
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AItem_Pickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItem_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when player overlap this actor
void AItem_Pickup::OverlapHandler(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA<ACSPlayer>())
	{
		this->Destroy();
	}
}

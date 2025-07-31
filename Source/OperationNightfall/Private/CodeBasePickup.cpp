// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeBasePickup.h"

// Sets default values
ACodeBasePickup::ACodeBasePickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Create and attach the box collider
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f)); // Adjust size as needed
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetGenerateOverlapEvents(true);

}


// Called when the game starts or when spawned
void ACodeBasePickup::BeginPlay()
{
	Super::BeginPlay();
	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACodeBasePickup::OnOverlap);
	}
}


// Called every frame
void ACodeBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACodeBasePickup::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Pickup hit"))
		if (OtherActor && OtherActor->Implements<UPlayerPickupInterface>())
		{

			TScriptInterface<IPlayerPickupInterface> PickupInterface = OtherActor;
			if (PickupInterface && PickupInterface->CanPickup()) {
				DoPickup(PickupInterface);
				PostPickup();
			}

		}

}

void ACodeBasePickup::DoPickup(TScriptInterface<IPlayerPickupInterface> PickupInterface)
{

}

void ACodeBasePickup::PostPickup()
{

}



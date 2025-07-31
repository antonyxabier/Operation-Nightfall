// Fill out your copyright notice in the Description page of Project Settings.


#include "KillPlane.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AKillPlane::AKillPlane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	KillBox = CreateDefaultSubobject<UBoxComponent>(TEXT("KillBox"));
	RootComponent = KillBox;
	KillBox->SetCollisionProfileName(TEXT("Trigger"));
	KillBox->SetBoxExtent(FVector(1000.0f, 1000.0f, 100.0f));
	KillBox->SetHiddenInGame(false);
	//KillBox->bVisualizeComponent = true;

}

// Called when the game starts or when spawned
void AKillPlane::BeginPlay()
{
	Super::BeginPlay();
	KillBox->OnComponentBeginOverlap.AddDynamic(this, &AKillPlane::OnOverlapBegin);
	
}

void AKillPlane::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter && PlayerCharacter->IsPlayerControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("Player hit kill plane. Respawning..."));
		UGameplayStatics::ApplyDamage(PlayerCharacter, 9999.0f, nullptr, this, nullptr);
		/*APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController());
		if (PC)
		{
			FName CurrentLevel = *UGameplayStatics::GetCurrentLevelName(this, true);
			UGameplayStatics::OpenLevel(this, CurrentLevel);
		}*/
	}

}




// Called every frame
void AKillPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


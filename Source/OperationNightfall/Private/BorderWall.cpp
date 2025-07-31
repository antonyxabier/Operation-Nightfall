// Fill out your copyright notice in the Description page of Project Settings.


#include "BorderWall.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "UI/CombatZoneWarningWidget.h"
#include "TimerManager.h"

// Sets default values
ABorderWall::ABorderWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WallTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("WallTrigger"));
	RootComponent = WallTrigger;
	WallTrigger->SetCollisionProfileName("Trigger");
	WallTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABorderWall::OnOverlapBegin);
	WallTrigger->OnComponentEndOverlap.AddDynamic(this, &ABorderWall::OnOverlapEnd);
	WallTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WallTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WallTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);


}

// Called when the game starts or when spawned
void ABorderWall::BeginPlay()
{
	Super::BeginPlay();
	
	if (CombatZoneWidgetClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		if (PC)
		{
			CombatZoneWidgetInstance = Cast<UCombatZoneWarningWidget>(
				CreateWidget<UUserWidget>(PC, CombatZoneWidgetClass));

			if (CombatZoneWidgetInstance)
			{
				CombatZoneWidgetInstance->AddToViewport();
				CombatZoneWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void ABorderWall::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
									UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//ACharacter* PlayerChar = Cast<ACharacter>(OtherActor);
	//if (PlayerChar && PlayerChar->IsPlayerControlled())
	//{
	//	GetWorld()->GetTimerManager().SetTimer(OutOfBoundsTimer, this, &ABorderWall::RespawnPlayer, OutOfBoundsTime, false);
	//	UE_LOG(LogTemp, Warning, TEXT("Player left bounds! Starting timer."));
	//}
	ACharacter* PlayerChar = Cast<ACharacter>(OtherActor);
	if (PlayerChar && PlayerChar->IsPlayerControlled())
	{
		if (CombatZoneWidgetInstance)
		{
			CombatZoneWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		}

		GetWorld()->GetTimerManager().SetTimer(OutOfBoundsTimer, this, &ABorderWall::OutOfBoundsDeath, OutOfBoundsTime, false);

		// Start ticking countdown
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				GetWorld()->GetTimerManager().SetTimer(
					CountdownUpdateHandle, this, &ABorderWall::UpdateCountdown, 0.1f, true);
			});
	}
}

void ABorderWall::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
									UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//ACharacter* PlayerChar = Cast<ACharacter>(OtherActor);
	//if (PlayerChar && PlayerChar->IsPlayerControlled())
	//{
	//	GetWorld()->GetTimerManager().ClearTimer(OutOfBoundsTimer);
	//	UE_LOG(LogTemp, Warning, TEXT("Player returned in bounds! Timer canceled."));
	//}

	ACharacter* PlayerChar = Cast<ACharacter>(OtherActor);
	if (PlayerChar && PlayerChar->IsPlayerControlled())
	{
		GetWorld()->GetTimerManager().ClearTimer(OutOfBoundsTimer);
		GetWorld()->GetTimerManager().ClearTimer(CountdownUpdateHandle);

		if (CombatZoneWidgetInstance)
		{
			CombatZoneWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ABorderWall::OutOfBoundsDeath()
{
	ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (PlayerChar)
	{
		UGameplayStatics::ApplyDamage(PlayerChar, 9999.0f, nullptr, this, nullptr);
	}

	if (CombatZoneWidgetInstance)
	{
		CombatZoneWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}

	GetWorld()->GetTimerManager().ClearTimer(CountdownUpdateHandle);
}


// Called every frame
void ABorderWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABorderWall::UpdateCountdown()
{
	if (CombatZoneWidgetInstance)
	{
		float TimeRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(OutOfBoundsTimer);
		CombatZoneWidgetInstance->SetSecondsRemaining(TimeRemaining);
	}
}


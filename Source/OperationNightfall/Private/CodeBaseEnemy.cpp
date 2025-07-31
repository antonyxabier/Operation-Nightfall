// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeBaseEnemy.h"
#include "AIC_CodeEnemyController.h"
#include "AIController.h"
#include "BaseRifle.h"
#include <Kismet/GameplayStatics.h>

ACodeBaseEnemy::ACodeBaseEnemy()
{
	AIControllerClass = AAIC_CodeEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	WeaponChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponChildActor"));
    WeaponChildActor->SetupAttachment(GetMesh(), "PlaceWeaponHere");
	
}

void ACodeBaseEnemy::BeginPlay()
{
    Super::BeginPlay();

    if (HealthComponent)
    {
        HealthComponent->OnHurt.AddDynamic(this, &ACodeBaseEnemy::ReactToDamage);
    }
   
   

    if (WeaponChildActor)
    {
        EquippedRifle = Cast<ABaseRifle>(WeaponChildActor->GetChildActor());
        if (EquippedRifle)
        {
            EquippedRifle->SetOwner(this);
		
        }
    }
    
   
    if (BehaviorTreeAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemy has a Behavior Tree asset assigned."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Behavior Tree asset is missing from enemy!"));
    }

    CharacterAnimation->OnDeathEnded.AddDynamic(this, &ACodeBaseEnemy::HandleDeath);
}

void ACodeBaseEnemy::Attack()
{
    if (EquippedRifle)
    {
        EquippedRifle->Attack();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemy has no weapon assigned!"));
    }
	//WeaponObject->Attack();
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }
   
}

void ACodeBaseEnemy::StartFiring()
{
    if (!FireRateTimer.IsValid())
    {
        GetWorldTimerManager().SetTimer(FireRateTimer, this, &ACodeBaseEnemy::Attack, FireRate, true);
    }
}

void ACodeBaseEnemy::StopFiring()
{
    GetWorldTimerManager().ClearTimer(FireRateTimer);
}


void ACodeBaseEnemy::ReactToDamage(float Damage)
{
    // Play hurt animation, trigger VFX, scream, etc.
}

void ACodeBaseEnemy::HandleDeath() {
    Destroy();
}



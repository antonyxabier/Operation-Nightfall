// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeEnemy_Heavy.h"
#include <Kismet/GameplayStatics.h>
#include <Projectile.h>

ACodeEnemy_Heavy::ACodeEnemy_Heavy()
{
	FireRate = 1.5f;
	MaxHealth = 200.0f;
	BaseDamage = 30.0f;
}

void ACodeEnemy_Heavy::BeginPlay()
{
    Super::BeginPlay();
}

void ACodeEnemy_Heavy::Attack()
{
    if (!EquippedRifle) return;

    AController* LocalController = GetController();
    if (!LocalController) return;

    FVector MuzzleLocation = EquippedRifle->GetMuzzleLocation();
    FRotator FireRotation = EquippedRifle->GetFireRotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = this;

    AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(
        EquippedRifle->ProjectileClass, MuzzleLocation, FireRotation, SpawnParams);

    if (SpawnedProjectile)
    {
        SpawnedProjectile->DamageAmount *= DamageMultiplier;

        UE_LOG(LogTemp, Warning, TEXT("Heavy Enemy fired projectile with modified damage: %f"), SpawnedProjectile->DamageAmount);
    }

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }
}



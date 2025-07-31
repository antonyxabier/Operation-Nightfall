// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Granade.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API AGranade : public AProjectile
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AGranade();
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};


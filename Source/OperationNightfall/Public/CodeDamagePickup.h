// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CodeBasePickup.h"
#include "NiagaraFunctionLibrary.h" 
#include "NiagaraComponent.h"
#include "Sound/SoundBase.h"
#include "CodeDamagePickup.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API ACodeDamagePickup : public ACodeBasePickup
{
	GENERATED_BODY()
	
public:
    ACodeDamagePickup();
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
    UParticleSystemComponent* PickupEffect;
    void OnConstruction(const FTransform& Transform);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
    UParticleSystem* PickupEffectAsset;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    UNiagaraSystem* PickupNiagaraEffect;

    // Damage amount applied when picked up
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
    float DamageAmount;

    void DestroyObject();

    //play sound
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    USoundBase* ExplosionSound;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    USoundBase* hurtSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Behavior",
        meta = (Tooltip = "If true, this pickup will destroy itself after applying damage or being hit by a projectile (like a landmine). If false, it will persist (like a fire hazard)."))
    bool bDestroyOnDamageApplied;

    // Override pickup logic to apply damage
    virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
       
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CodeDamagePickup.h"
#include "Sound/SoundBase.h"
#include "CodeHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API ACodeHealthPickup : public ACodeDamagePickup
{

    GENERATED_BODY()
public:
    ACodeHealthPickup();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
    TObjectPtr<USoundBase> pickupSound;
protected:
    virtual void BeginPlay() override;
    virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;




};

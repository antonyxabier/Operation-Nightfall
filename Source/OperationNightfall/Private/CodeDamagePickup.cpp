// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeDamagePickup.h"
#include "BaseCharacter.h"
#include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BasePlayer.h"

ACodeDamagePickup::ACodeDamagePickup() {
    PrimaryActorTick.bCanEverTick = false;
    // Create the particle system component
    PickupEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PickupEffect"));
    PickupEffect->SetTemplate(PickupEffectAsset);
    // Set default damage amount
    DamageAmount = 0.5f;

    bDestroyOnDamageApplied = true;
    hurtSound = nullptr;
}


void ACodeDamagePickup::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);



    PickupEffect->SetupAttachment(CollisionBox);
    PickupEffect->SetAutoActivate(true);


}

void ACodeDamagePickup::BeginPlay()
{
    Super::BeginPlay();

    if (PickupEffectAsset)
    {

        PickupEffect->ActivateSystem();

    }
}

void ACodeDamagePickup::DestroyObject(){


    if (ExplosionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            ExplosionSound,
            GetActorLocation()
        );

        UGameplayStatics::PlaySound2D(this, ExplosionSound);
    }


    if (PickupNiagaraEffect)
    {



        UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
            PickupNiagaraEffect,
            RootComponent,           // Attach to root or another component
            NAME_None,               // Optional socket name
            FVector::ZeroVector,     // Relative location
            FRotator::ZeroRotator,   // Relative rotation
            EAttachLocation::KeepRelativeOffset,
            true,                    // AutoDestroy
            true                     // AutoActivate
        );
        NiagaraComp->SetVariableInt("LoopCount", 1);
        NiagaraComp->SetVariableFloat("EffectDuration", 1.0f);
    }

    SetLifeSpan(1.0f); // Allow time for the effect to finish
}

void ACodeDamagePickup::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    
        ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor);
        if (Character)
        {
            // Call OnHurt function to process damage
            ABasePlayer* HitPlayer = Cast<ABasePlayer>(Character);
            if (HitPlayer && HitPlayer->bGodMode) {
                // nothing happens in god mode
            }
            else {
                UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, GetInstigatorController(), this, nullptr);
                UGameplayStatics::PlaySound2D(this, hurtSound);
                if (bDestroyOnDamageApplied) {
                    DestroyObject();
                }
            }
            
            UE_LOG(LogTemp, Warning, TEXT("Damage Pickup activated! Applied %f damage."), DamageAmount);
        }
        AProjectile *projectile = Cast<AProjectile>(OtherActor);
        if (projectile) {
            DestroyObject();
        }
       
    


}
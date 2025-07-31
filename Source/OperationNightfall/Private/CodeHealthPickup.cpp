// Fi
// Fill out your copyright notice in the Description page of Project Settings.

#include "CodeHealthPickup.h"
#include "PlayerPickupInterface.h"
#include "BasePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"



ACodeHealthPickup::ACodeHealthPickup()
{

   // Set default damage amount
    DamageAmount = -0.5f;
}


void ACodeHealthPickup::BeginPlay()
{
    Super::BeginPlay();


}


void ACodeHealthPickup::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    UE_LOG(LogTemp, Warning, TEXT("Health Pickup hit"))
        if (OtherActor && OtherActor->Implements<UPlayerPickupInterface>())
        {
            IPlayerPickupInterface* PickupInterface = Cast<IPlayerPickupInterface>(OtherActor);
            if (PickupInterface && PickupInterface->CanPickup())
            {
                ABasePlayer* Character = Cast<ABasePlayer>(OtherActor);
                if (Character)
                {
                    float HealAmount = -DamageAmount;
                    UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, GetInstigatorController(), this, nullptr);
                    UGameplayStatics::PlaySound2D(this, pickupSound);
                    UE_LOG(LogTemp, Warning, TEXT("Health Pickup activate Healed %f health."), HealAmount);
                    Destroy();
                }
            }

            if (PickupEffectAsset)
            {
                UParticleSystemComponent* SpawnedEffect = UGameplayStatics::SpawnEmitterAttached(
                    PickupEffectAsset,                  // Your UParticleSystem*
                    Cast<USceneComponent>(OtherActor->GetRootComponent()), // Attach to OtherActor’s root
                    NAME_None,                     // Optional socket name
                    FVector::ZeroVector,           // Relative location
                    FRotator::ZeroRotator,         // Relative rotation
                    EAttachLocation::KeepRelativeOffset,
                    true                           // Auto destroy
                );
                UE_LOG(LogTemp, Warning, TEXT("Health Pickup effect."));
                FTimerHandle EffectDestroyTimer;
                GetWorld()->GetTimerManager().SetTimer(EffectDestroyTimer, [SpawnedEffect]()
                    {
                        if (SpawnedEffect)
                        {
                            SpawnedEffect->DeactivateSystem();
                            SpawnedEffect->DestroyComponent();
                        }
                    }, 1.0f, false);  // 2-second lifespan

            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("Health Pickup no asset"));
            }


        }
}

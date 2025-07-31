#include "CodeAmmoPickup.h"
#include "BasePlayer.h"
#include "Kismet/GameplayStatics.h"

ACodeAmmoPickup::ACodeAmmoPickup() {
    DamageAmount = 0; // override inherited damage amount since this object doesn't affect health in any way
}

void ACodeAmmoPickup::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->Implements<UPlayerPickupInterface>()) {
        IPlayerPickupInterface* PickupInterface = Cast<IPlayerPickupInterface>(OtherActor);
        if (PickupInterface && PickupInterface->CanPickup()) {
            ABasePlayer* Player = Cast<ABasePlayer>(OtherActor);
            if (Player) {
                int AmmoGiven = FMath::RandRange(MinAmmo, MaxAmmo);
                Player->PickupAmmo(static_cast<float>(AmmoGiven));
                UGameplayStatics::PlaySound2D(this, ammoSound);
                Destroy();
            }
        }
    }
}
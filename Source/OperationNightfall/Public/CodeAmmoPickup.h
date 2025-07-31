#pragma once

#include "CoreMinimal.h"
#include "CodeHealthPickup.h"
#include "CodeAmmoPickup.generated.h"

UCLASS()
class OPERATIONNIGHTFALL_API ACodeAmmoPickup : public ACodeHealthPickup
{
	GENERATED_BODY()
	
public:
	ACodeAmmoPickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TObjectPtr<USoundBase> ammoSound;

protected:
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	int MinAmmo = 8;
	int MaxAmmo = 12;
};

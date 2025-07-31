#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SpotLightComponent.h"
#include "BaseRifle.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRifleAction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAmmoChanged, float, Curr, float, Max, float, Reserve);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCallOnReloadStart);

UCLASS()
class OPERATIONNIGHTFALL_API ABaseRifle : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseRifle();
	virtual void Tick(float DeltaTime) override;

	void Attack(); // attack function

	UFUNCTION(BlueprintPure, Category = Weapon)
	FVector GetSource();

	bool CanShoot() const; // checks if we can shoot

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void UseAmmo(); // reduces ammo after attacking

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ActionStopped();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void RequestReload(); // check if we can reload

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ReloadAmmo(); // reload funciton

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AddAmmo(float amount); // adds ammo to reserve pool

	// class delegates
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Attack")
	FOnRifleAction OnRifleAttack;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Attack")
	FOnRifleAction OnActionStopped;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Ammo Changed")
	FCallOnReloadStart OnReloadStart;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Ammo Changed")
	FOnAmmoChanged OnAmmoChanged;

	UFUNCTION(BlueprintCallable)
	void OwnerDied();

	UFUNCTION(BlueprintCallable)
	FVector GetMuzzleLocation() const { return baseRifleMesh->GetSocketLocation(WeaponSocketName); }

	UFUNCTION(BlueprintCallable)
	FRotator GetFireRotation() const { return baseRifleMesh->GetSocketRotation(WeaponSocketName); }

	bool Alive = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AActor> GranadeClass;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* baseRifleMesh; // skeletal mesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	FName WeaponSocketName; // socket name for rifle muzzle

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	APawn* ParentPawn; // parent pawn

	float MaxAmmo = 6; // amount of ammo rifle can hold at a time
	float ReserveAmmo = 18; // total ammo player holds (3 times Max ammo for start)

	FTimerHandle timer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	float ResetTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
	USpotLightComponent* FlashlightComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
	bool bFlashlightOn;

public:	
	// flashlight funcitons
	UFUNCTION(BlueprintCallable, Category = "Flashlight")
	void ToggleFlashlight();

	UFUNCTION(BlueprintCallable, Category = "Flashlight")
	void TurnFlashlightOn();

	UFUNCTION(BlueprintCallable, Category = "Flashlight")
	void TurnFlashlightOff();

	// todo change to enum
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AmmoType =1;

private:
	bool ActionHappening;
	float CurrAmmo;

	void HandleActionFinished();
};
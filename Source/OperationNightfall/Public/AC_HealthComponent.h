// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHurt, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, float, Damage);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERATIONNIGHTFALL_API UAC_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_HealthComponent();
	float CurrentHealth;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void HandleDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);


	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHurt OnHurt;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeath OnDeath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	bool bIsDead;

	UFUNCTION()
	void RestoreFullHealth();

};

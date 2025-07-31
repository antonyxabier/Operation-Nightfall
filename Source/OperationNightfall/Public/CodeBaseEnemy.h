// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "AIC_CodeEnemyController.h"
#include "CodeBaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API ACodeBaseEnemy : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	ACodeBaseEnemy();

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	ABaseRifle* EquippedRifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UChildActorComponent* WeaponChildActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float BaseDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float FireRate = 1.0f;

	FTimerHandle FireRateTimer;

	UFUNCTION(BlueprintCallable)
	virtual void Attack();

	UPROPERTY(EditAnywhere, Category = "SFX")
	class USoundBase* FireSound;

	UFUNCTION()
	void StartFiring();

	UFUNCTION()
	void StopFiring();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void ReactToDamage(float Damage);

	UFUNCTION()
	void HandleDeath();
};

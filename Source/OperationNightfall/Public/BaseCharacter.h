// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseRifle.h"
#include "CharacterAnimation.h"
#include "AC_HealthComponent.h"
#include "GenericTeamAgentInterface.h"
#include "BaseCharacter.generated.h"


UCLASS(Abstract)
class OPERATIONNIGHTFALL_API ABaseCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team", meta = (AllowPrivateAccess = "true"))
	FGenericTeamId TeamID;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AActor> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildActorComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class ABaseRifle* WeaponObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UCharacterAnimation* CharacterAnimation;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UAC_HealthComponent* HealthComponent;

	UFUNCTION(BlueprintCallable)
	virtual void HandleHurt(float ratio);

	UFUNCTION(BlueprintCallable)
	virtual void HandleDeathStart(float ratio);
	
	void OnReloadPressed();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> baseCharacterMesh;

	bool alive = true;
	
};

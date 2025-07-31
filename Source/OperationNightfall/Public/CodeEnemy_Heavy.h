// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CodeBaseEnemy.h"
#include "CodeEnemy_Heavy.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API ACodeEnemy_Heavy : public ACodeBaseEnemy
{
	GENERATED_BODY()
	
public:

	ACodeEnemy_Heavy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float DamageMultiplier = 1.5f; 


	virtual void Attack() override;


	void BeginPlay() override;
};

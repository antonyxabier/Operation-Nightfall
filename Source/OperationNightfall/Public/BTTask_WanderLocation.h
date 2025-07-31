// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_WanderLocation.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API UBTTask_WanderLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_WanderLocation();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Wander")
	float WanderRadius = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Wander")
	float WanderInterval = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DestinationKey;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AIC_CodeEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API AAIC_CodeEnemyController : public AAIController
{
	GENERATED_BODY()
	

public:
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTreeComponent* BehaviorTreeComp;
	AAIC_CodeEnemyController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;
};

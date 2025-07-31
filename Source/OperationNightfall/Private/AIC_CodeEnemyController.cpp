// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_CodeEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "CodeBaseEnemy.h"


AAIC_CodeEnemyController::AAIC_CodeEnemyController()
{
	
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));

	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	SetPerceptionComponent(*AIPerceptionComp);
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if(SightConfig)
	{
		SightConfig->SightRadius = 2000.0f; 
		SightConfig->LoseSightRadius = 1800.0f; 
		SightConfig->PeripheralVisionAngleDegrees = 90.0f; 
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 1000.0f; 
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
		AIPerceptionComp->ConfigureSense(*SightConfig);
		AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
		AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AAIC_CodeEnemyController::OnTargetPerceptionUpdated);
	}

}
void AAIC_CodeEnemyController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	Super::SetGenericTeamId(NewTeamID);
	if (UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(GetWorld()))
	{
		PerceptionSystem->UpdateListener(*GetPerceptionComponent());
	}

}
void AAIC_CodeEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(InPawn))
	{
		
		SetGenericTeamId(TeamAgent->GetGenericTeamId());
	}

	ACodeBaseEnemy* CodeBaseEnemy = Cast<ACodeBaseEnemy>(InPawn);
	if (CodeBaseEnemy && CodeBaseEnemy->BehaviorTreeAsset)
	{
		if(UseBlackboard(CodeBaseEnemy->BehaviorTreeAsset->BlackboardAsset, BlackboardComp))
		{
			BehaviorTreeComp->StartTree(*CodeBaseEnemy->BehaviorTreeAsset);
			UE_LOG(LogTemp, Log, TEXT("Behavior Tree started for %s"), *InPawn->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to use blackboard for %s"), *InPawn->GetName());
		}
	}
	
}

void AAIC_CodeEnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		
		GetBlackboardComponent()->SetValueAsObject("TargetActor", Actor);
	}
	else
	{
		
		GetBlackboardComponent()->ClearValue("TargetActor");
	}
}



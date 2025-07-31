// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EnemyAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CodeBaseEnemy.h"
#include "BaseRifle.h" 
#include "Kismet/GameplayStatics.h"


UBTTask_EnemyAttack::UBTTask_EnemyAttack()
{
	NodeName = "Enemy Attack";
	bNotifyTick = true; 
}

//EBTNodeResult::Type UBTTask_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
//{
//	return EBTNodeResult::InProgress;
//}

EBTNodeResult::Type UBTTask_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn) return EBTNodeResult::Failed;

	ACodeBaseEnemy* Enemy = Cast<ACodeBaseEnemy>(ControlledPawn);
	if (!Enemy) return EBTNodeResult::Failed;

	
	if (Enemy->EquippedRifle)
	{
		Enemy->Attack();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy has no weapon assigned!"));
	}
	return EBTNodeResult::Succeeded;
}


//void UBTTask_EnemyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//{
//	AActor* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
//	if (!ControlledPawn)
//	{
//		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
//		return;
//	}
//	ACodeBaseEnemy* Enemy = Cast<ACodeBaseEnemy>(ControlledPawn);
//	if (!Enemy || !Enemy->EquippedRifle)
//	{
//		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
//		return;
//	}
//	Enemy->Attack();
//}
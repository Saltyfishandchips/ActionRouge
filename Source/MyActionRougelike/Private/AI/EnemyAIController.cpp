// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please Assgin BehaviorTree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}
	
	//APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//if (MyPawn)
	//{
	//	GetBlackboardComponent()->SetValueAsVector("MoveToLocatioin", MyPawn->GetActorLocation());
	//	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	//}
}

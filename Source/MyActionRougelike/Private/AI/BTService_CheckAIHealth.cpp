// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckAIHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AttributeComponent.h"
#include "AIController.h"

UBTService_CheckAIHealth::UBTService_CheckAIHealth()
{
	HealthThreshold = 30.f;
}


void UBTService_CheckAIHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//通过BTService获取AI自身的生命值，如果生命值低于一定阈值，则之后AI原理Character并恢复生命值
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (ensure(AIPawn))
	{
		UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(AIPawn);
		if (ensure(AttributeComp))
		{
			//获取EmenyAI的Attribute然后获得Health
			float AIHealth = AttributeComp->GetHealth();
			//设置key值，返回bool，当health低于阈值则置为真
			if (ensure(AIHealth))
			{
				UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
				
				bool bIsLowHealth = AIHealth < HealthThreshold;

				BlackboardComp->SetValueAsBool(CheckHealthLow.SelectedKeyName, bIsLowHealth);
				
				/*AIHealth < HealthThreshold ? BlackboardComp->SetValueAsBool(CheckHealthLow.SelectedKeyName, true) : BlackboardComp->SetValueAsBool(CheckHealthLow.SelectedKeyName, false);*/
			}
		}
	}


}

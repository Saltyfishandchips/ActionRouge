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

	//ͨ��BTService��ȡAI���������ֵ���������ֵ����һ����ֵ����֮��AIԭ��Character���ָ�����ֵ
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (ensure(AIPawn))
	{
		UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(AIPawn);
		if (ensure(AttributeComp))
		{
			//��ȡEmenyAI��AttributeȻ����Health
			float AIHealth = AttributeComp->GetHealth();
			//����keyֵ������bool����health������ֵ����Ϊ��
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

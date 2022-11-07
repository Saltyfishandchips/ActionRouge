// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_HealingSelf.h"
#include "AttributeComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTTaskNode_HealingSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* AIPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());

	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	//��ȡAI��Attribute���
	UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(AIPawn);

	if (ensure(AttributeComp))
	{
		//AI����Ѫ��
		AttributeComp->ApplyHealthChange(AIPawn, AttributeComp->GetHealthMax());
	}

	return EBTNodeResult::Succeeded;

}

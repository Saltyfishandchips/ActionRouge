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

	//获取AI的Attribute组件
	UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(AIPawn);

	if (ensure(AttributeComp))
	{
		//AI回满血量
		AttributeComp->ApplyHealthChange(AIPawn, AttributeComp->GetHealthMax());
	}

	return EBTNodeResult::Succeeded;

}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckAIHealth.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONROUGELIKE_API UBTService_CheckAIHealth : public UBTService
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	float HealthThreshold;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector CheckHealthLow;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UBTService_CheckAIHealth();
};

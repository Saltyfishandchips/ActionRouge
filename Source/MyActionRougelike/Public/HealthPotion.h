// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerupBasic.h"
#include "HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONROUGELIKE_API AHealthPotion : public APowerupBasic
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, Category = "HealthPotion")
	int32 CreditCost;


public:

	void Interaction_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

	AHealthPotion();
};

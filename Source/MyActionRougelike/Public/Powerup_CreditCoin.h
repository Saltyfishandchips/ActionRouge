// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerupBasic.h"
#include "Powerup_CreditCoin.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONROUGELIKE_API APowerup_CreditCoin : public APowerupBasic
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "CreditCoin")
	int32 CreditCost;

public:

	virtual void Interaction_Implementation(APawn* InstigatorPawn) override;

	APowerup_CreditCoin();
};

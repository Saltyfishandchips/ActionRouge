// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerupBasic.h"
#include "Powerup_Action.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONROUGELIKE_API APowerup_Action : public APowerupBasic
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "Powerup")
	TSubclassOf<UAction> ActionToGrant;

public:

	virtual void Interaction_Implementation(APawn* InstigatorPawn) override;

};

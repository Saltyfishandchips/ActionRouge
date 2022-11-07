// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBallBasic.h"
#include "DashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONROUGELIKE_API ADashProjectile : public AAttackBallBasic
{
	GENERATED_BODY()
	
protected:
	void Explode_Implementation() override;

	UPROPERTY(VisibleAnywhere)
	float TeleportDelay;

	UPROPERTY(VisibleAnywhere)
	float DetonateDelay;

	FTimerHandle TimerHandle_DelayedDetonate;

	virtual void BeginPlay() override;

	UFUNCTION()
	void TeleportInstigator();

public:
	ADashProjectile();

};

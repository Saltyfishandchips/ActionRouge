// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionEffect.h"
#include "ActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONROUGELIKE_API UActionEffect_Thorns : public UActionEffect
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Throns")
	float RefectFraction;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewAttributeValue, float Delta);
	
public:
	virtual void StartAction_Implementation(AActor* Instigater) override;

	virtual void StopAction_Implementation(AActor* Instigater) override;

	UActionEffect_Thorns();

};

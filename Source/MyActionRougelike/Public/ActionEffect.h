// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "ActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONROUGELIKE_API UActionEffect : public UAction
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* instigator);

public:

	UFUNCTION(BlueprintCallable, Category = "Effect")
	float GetTimeRemaining() const;

	virtual void StartAction_Implementation(AActor* Instigater) override;

	virtual void StopAction_Implementation(AActor* Instigater) override;

	UActionEffect();
};

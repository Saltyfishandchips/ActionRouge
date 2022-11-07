// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBallBasic.h"
#include "GameplayTagContainer.h"
#include "MagicPorjectile.generated.h"

class UActionEffect;
/**
 * 
 */
UCLASS()
class MYACTIONROUGELIKE_API AMagicPorjectile : public AAttackBallBasic
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;

	//ע��FGamePlayTag����ָ�룬���Ҫ��.h�а���Tag��ͷ�ļ�
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
 	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UActionEffect> BurningActionClass;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	AMagicPorjectile();
};

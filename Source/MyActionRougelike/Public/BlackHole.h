// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBallBasic.h"
#include "BlackHole.generated.h"

//class URadialForceComponent;
class USphereComponent;
class URadialForceComponent;
/**
 * 
 */
UCLASS()
class MYACTIONROUGELIKE_API ABlackHole : public AAttackBallBasic
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "LifeSpawn")
	float LifeTime;

	UFUNCTION()
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* RadialForce;

	UFUNCTION()
	void BlackHoleEffect(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 num, bool thing, const FHitResult& Hit);

public:
	ABlackHole();
};

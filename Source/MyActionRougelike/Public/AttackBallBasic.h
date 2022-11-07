// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "AttackBallBasic.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UParticleSystem;
class UProjectileMovementComponent;
class UAudioComponent;

UCLASS()
class MYACTIONROUGELIKE_API AAttackBallBasic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttackBallBasic();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ParticleComp;

	//����������Ч
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UParticleSystem* ImpactFX;

	//Ͷ��������
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAudioComponent* ImpactSound;

	//��ɫ������ʱ�������
	UPROPERTY(EditAnywhere, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> ImpactShake;

	UPROPERTY(EditAnywhere, Category = "CameraShake")
	float ImpactShakeInnerRadius;

	UPROPERTY(EditAnywhere, Category = "CameraShake")
	float ImpactShakeOuterRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* ProjectileAudio;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Explode();


};

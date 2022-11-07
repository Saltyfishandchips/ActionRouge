// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicBall.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UParticleSystem;
class UAudioComponent;

UCLASS()
class MYACTIONROUGELIKE_API AMagicBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagicBall();

	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//����ӵ���ը������Ч
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	UParticleSystem* HitCharacter;

	//���Hit_World
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	UParticleSystem* HitWorld;

	//����˺�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DamageAmount;

	//���������壬����ײ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	//���������Ч���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;

	//���Ͷ�����������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* MagicBallAudio;

	//��ɫ������ʱ�������
	UPROPERTY(EditAnywhere, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, Category = "CameraShake")
	float ImpactShakeInnerRadius;

	UPROPERTY(EditAnywhere, Category = "CameraShake")
	float ImpactShakeOuterRadius;

	//���Ͷ�������ʱ������Ч��
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

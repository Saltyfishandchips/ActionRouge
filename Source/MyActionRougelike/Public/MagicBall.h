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
	//添加子弹爆炸粒子特效
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	UParticleSystem* HitCharacter;

	//添加Hit_World
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	UParticleSystem* HitWorld;

	//添加伤害
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DamageAmount;

	//添加网络格体，即碰撞盒
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	//添加粒子特效组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;

	//添加投射物物理组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* MagicBallAudio;

	//角色被击中时相机抖动
	UPROPERTY(EditAnywhere, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, Category = "CameraShake")
	float ImpactShakeInnerRadius;

	UPROPERTY(EditAnywhere, Category = "CameraShake")
	float ImpactShakeOuterRadius;

	//添加投射物击中时的物理效果
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

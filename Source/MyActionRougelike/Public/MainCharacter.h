// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "SInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BlackHole.h"
#include "ActionComponent.h"
#include "MainCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class UKismetMathLibrary;
class USceneComponent;
class UAttributeComponent;
class UActionComponent;

UCLASS()
class MYACTIONROUGELIKE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	//添加Actor组件
	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	//添加保存角色属性的ActorComponent中
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAttributeComponent* AttributeComp;

	//添加角色动作GAS的ActorComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UActionComponent* ActionComp;

	UPROPERTY(EditAnywhere)
	float AttackDelay;

protected:

	//移动函数
	UFUNCTION()
	void MoveForward(float value);

	UFUNCTION()
	void MoveRight(float value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();
	
	//冲刺函数
	UFUNCTION()
	void SprintStart();

	UFUNCTION()
	void SprintStop();
	
	/*角色交互函数*/
	//交互函数
	UFUNCTION()
	void PrimaryInteraction();

	/*角色攻击函数*/
	//普通攻击
	UFUNCTION()
	void PrimaryAttack();

	//黑洞攻击
	UFUNCTION()
	void BlackHoleAttack();

	//传送
	UFUNCTION()
	void TeleportAttack();


	//当角色死亡时，禁止玩家的输入
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//重写视线获取函数
	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

};

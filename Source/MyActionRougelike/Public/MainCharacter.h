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

	//���Actor���
	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	//��ӱ����ɫ���Ե�ActorComponent��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAttributeComponent* AttributeComp;

	//��ӽ�ɫ����GAS��ActorComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UActionComponent* ActionComp;

	UPROPERTY(EditAnywhere)
	float AttackDelay;

protected:

	//�ƶ�����
	UFUNCTION()
	void MoveForward(float value);

	UFUNCTION()
	void MoveRight(float value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();
	
	//��̺���
	UFUNCTION()
	void SprintStart();

	UFUNCTION()
	void SprintStop();
	
	/*��ɫ��������*/
	//��������
	UFUNCTION()
	void PrimaryInteraction();

	/*��ɫ��������*/
	//��ͨ����
	UFUNCTION()
	void PrimaryAttack();

	//�ڶ�����
	UFUNCTION()
	void BlackHoleAttack();

	//����
	UFUNCTION()
	void TeleportAttack();


	//����ɫ����ʱ����ֹ��ҵ�����
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//��д���߻�ȡ����
	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

};

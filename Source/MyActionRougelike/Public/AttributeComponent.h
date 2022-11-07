// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

//AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta
//���������Լ����¼�����ͬBeginOverlap֮���һ����������Ҫͨ��UE�ĺ���ж���
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, UAttributeComponent*, OwningComp, float, NewAttributeValue, float, Delta);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYACTIONROUGELIKE_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorcAlive(AActor* Actor);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Arrtibute")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Arrtibute")
	float HealthMax;

	/*UPROPERTY(ReplicatedUsing = "OnRep_IsAlive")
	bool bIsAlive;*/

	//ReplicatedUsing����֪ͨ�ͻ�����Actor״̬�ı仯����������
	//��NetMulticast��������˲����̬�ı仯��������Ͱ��ը������Ŀ�꣬����һ����ı�Actor��״̬

	//�������ʹ��UnReliable����Ϊ���ǽ�Health����Ϊ��Replicated,�ڿͻ���������������ζ����Եõ�Healthֵ
	//������MainCharacter�У�HealthChanged����Ӱ��Controller�����룬��������������ó�ΪReliable��
	//��Server�е���MultiCast������Լ����������пͻ��˵��ã�Client����������ڱ��ص��ã������������ҵ���

	//���⣬�ڴ��������У�����Ҫ����Actor������ԣ������ڴ���ɱ�У���ͼ��Ե�����ӱ�����
	//����ɫ����ڵ�ͼ��һ�ߣ����ɫ����Ҫֱ�������ӵ�״̬��ֱ��������㹻��
	//���ʹ��NetMulticast��Reliable�����ҵ����Ըı�ʱ���ùؿ��ڵ�����������Ҷ������ȡ�����Եı仯��������������˷�

	UFUNCTION(NetMulticast, Reliable)//@FIXME:��MainCharacter�е������Ƴ��󣬡�state���Ƴ�MainCharacter�󣬽�������ΪUnReliable
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);


	//ŭ��ֵ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Arrtibute")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Arrtibute")
	float RageMax;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);

public:

	/*---------------------
		Health�µĸ��������ı�
	----------------------*/
	UFUNCTION(BlueprintCallable)
	float GetHealth();

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth();

	UFUNCTION(BlueprintCallable)
	float GetHealthMax();

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool ApplyHealthChange(AActor* InstigatoActor, float Delta);
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;


	UFUNCTION(BlueprintCallable)
	float GetRage();

	UFUNCTION(BlueprintCallable)
	bool IsFullRage();

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;
};
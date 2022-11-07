// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYACTIONROUGELIKE_API ISGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//BlueprintImplementableEvent����ʵ��C++����ͼ��������C++��ʵ������ͼ����������ǲ���������C++��
	//BlueprintNativeEvent����ʵ��C++������ͼ�⣬ͬ�������һ�����ط��������ط���Ϊ �����ĺ�����+_Implementation

	//�������涨��ĺ�����������Ҫ���������е���ʱ�����Ƕ���Interact_Implementation()��һ�麯��
	
	//UFUNCTION(BlueprintNativeEvent)
	//void Interact(APawn* InstigatorPawn);
	
	//BlueprintCallable����ʹ�øýӿں������Ա���ͼ����
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interaction(APawn* InstigatorPawn);

	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractText(APawn* InstigatorPawn);
};

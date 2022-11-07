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
	//BlueprintImplementableEvent用于实现C++调蓝图（声明在C++，实现在蓝图），因此我们不适用其在C++中
	//BlueprintNativeEvent除了实现C++调用蓝图外，同样会调用一个本地方法，本地方法为 声明的函数名+_Implementation

	//对于下面定义的函数，当我们要在其他类中调用时，则是定义Interact_Implementation()这一虚函数
	
	//UFUNCTION(BlueprintNativeEvent)
	//void Interact(APawn* InstigatorPawn);
	
	//BlueprintCallable可以使得该接口函数可以被蓝图调用
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interaction(APawn* InstigatorPawn);

	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractText(APawn* InstigatorPawn);
};

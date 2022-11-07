// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ActionComponent.generated.h"

class UAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FonACtionStateChanged, UActionComponent*, OwningComp, UAction*, Action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYACTIONROUGELIKE_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//游戏标签，用于限制释放技能或行动时释放其他攻击，同时由于我们这里没有使用指针，需要在h文件中直接包含该头文件
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instigater, TSubclassOf<UAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(UAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool StartActionByName(AActor* Instigater, FName ActionName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool StopActionByName(AActor* Instigater, FName ActionName);

	UFUNCTION()
	UAction* GetAction(TSubclassOf<UAction> ActionClass) const;

	// Sets default values for this component's properties
	UActionComponent();

protected:

	//ServerRPC，将StartAction放在Server中
	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigater, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigater, FName ActionName);

	//暴露给蓝图的技能数组
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UAction>> DefaultActions;

	//当前组件拥有技能的数组
	UPROPERTY(BlueprintReadOnly,Replicated)
	TArray<UAction*> Actions;


	// Called when the game starts
	virtual void BeginPlay() override;

public:	


	UPROPERTY(BlueprintAssignable)
	FonACtionStateChanged OnACtionStarted;

	UPROPERTY(BlueprintAssignable)
	FonACtionStateChanged OnActionStopped;


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
		
};

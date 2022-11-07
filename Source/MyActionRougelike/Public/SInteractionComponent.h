// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorldUserWidget.h"
#include "SInteractionComponent.generated.h"

class UWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYACTIONROUGELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//这里需要注意，ActorComponet组件中需要调的函数要设置为public，这可以使其被其他对象进行调用
	void PrimaryInteraction();

	//设置射线检测以获取目标的位置
	FVector AttackLineDetection();

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

protected:

	//Reliable 最终肯定会到达，如果没有收到响应将会一直重发(TCP/IP)
	//Unreliable 将不保证package是否会到达，同时也不会重发(UDP)

	//建立一个SeverRpc:Client->Sever，用于只在Sever中运行这一函数
	//在RPC中传入参数，当客户端将参数传进去时，其传入的是一个ID，这个ID对应在服务端中改参数的内存位置
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFcous);

	// Called when the game starts
	virtual void BeginPlay() override;

	void FindBestInteractable();

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TranceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	//不能使用下面这种方式，因为ECollisionChannel在UE4中属于Enum，因此我们需要UE4中的Enum将其暴露
	/*ECollisionChannel CollisionChannel;*/
	TEnumAsByte<ECollisionChannel> CollisionChannel;


	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	UWorldUserWidget* DefaultWidgetInstance;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

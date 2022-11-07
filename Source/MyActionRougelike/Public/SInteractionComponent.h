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
	//������Ҫע�⣬ActorComponet�������Ҫ���ĺ���Ҫ����Ϊpublic�������ʹ�䱻����������е���
	void PrimaryInteraction();

	//�������߼���Ի�ȡĿ���λ��
	FVector AttackLineDetection();

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

protected:

	//Reliable ���տ϶��ᵽ����û���յ���Ӧ����һֱ�ط�(TCP/IP)
	//Unreliable ������֤package�Ƿ�ᵽ�ͬʱҲ�����ط�(UDP)

	//����һ��SeverRpc:Client->Sever������ֻ��Sever��������һ����
	//��RPC�д�����������ͻ��˽���������ȥʱ���䴫�����һ��ID�����ID��Ӧ�ڷ�����иĲ������ڴ�λ��
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
	//����ʹ���������ַ�ʽ����ΪECollisionChannel��UE4������Enum�����������ҪUE4�е�Enum���䱩¶
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

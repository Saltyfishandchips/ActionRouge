// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//����ײ���������
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	//���䶥����������
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;

	//�������縴�ƣ���ʱ�������ͻ���
	SetReplicates(true);
}


void ASItemChest::Interaction_Implementation(APawn* InstigatorPawn)
{
	bLidOpen = !bLidOpen;

	//ReplicatedUsingĬ��ֻ�ڿͻ������У��������ڷ�������У���˷����Ҳ��Ҫ�ֶ�ִ��OnRep_OpenLid()����
	OnRep_OpenLid();

}


void ASItemChest::OnActorLoaded_Implementation()
{
	OnRep_OpenLid();
}

//��bLidOpen�ı�ʱ���ͻ����Զ�ִ�����溯����������˲�ִ�У������Ҫ�ֶ�ִ��
void ASItemChest::OnRep_OpenLid()
{
	//����������������ת������������ӵ�����
	float CurrentPitch = bLidOpen ? TargetPitch : 0.f;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
}



void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//ÿ��bLidOpen�ı�ʱ�����䷢�͸����пͻ��ˣ�û���κ��������ƣ��ı��ֱ�ӷ���
	DOREPLIFETIME(ASItemChest, bLidOpen);
}

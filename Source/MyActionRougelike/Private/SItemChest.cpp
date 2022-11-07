// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//宝箱底部的网格体
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	//宝箱顶部的网格体
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;

	//开启网络复制，随时更新至客户端
	SetReplicates(true);
}


void ASItemChest::Interaction_Implementation(APawn* InstigatorPawn)
{
	bLidOpen = !bLidOpen;

	//ReplicatedUsing默认只在客户端运行，而不会在服务端运行，因此服务端也需要手动执行OnRep_OpenLid()函数
	OnRep_OpenLid();

}


void ASItemChest::OnActorLoaded_Implementation()
{
	OnRep_OpenLid();
}

//当bLidOpen改变时，客户端自动执行下面函数，但服务端不执行，因此需要手动执行
void ASItemChest::OnRep_OpenLid()
{
	//设置了物体的相对旋转，相对于所连接的物体
	float CurrentPitch = bLidOpen ? TargetPitch : 0.f;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
}



void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//每当bLidOpen改变时，将其发送给所有客户端，没有任何条件限制，改变就直接发送
	DOREPLIFETIME(ASItemChest, bLidOpen);
}

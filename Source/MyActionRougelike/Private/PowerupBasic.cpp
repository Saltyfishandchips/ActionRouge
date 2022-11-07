// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupBasic.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
APowerupBasic::APowerupBasic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.f;
	bIsActive = true;
	
	bReplicates = true;
	SetReplicates(bReplicates);
}


void APowerupBasic::Interaction_Implementation(APawn* InstigatorPawn) //基类的互动操作，需要到具体的子类中进行override
{

}

FText APowerupBasic::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}



void APowerupBasic::ShowPowerup()
{
	SetPowerupState(true);
}

void APowerupBasic::HideAndCooldownPowerup()
{
	SetPowerupState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &APowerupBasic::ShowPowerup, RespawnTime);
}

void APowerupBasic::SetPowerupState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_IsActive();//在服务端中，由于RepNotify只在客户端运行，因此在服务端中我们也需要调用。
}

void APowerupBasic::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	// Set visibility on root and all children
	RootComponent->SetVisibility(bIsActive, true);
}


void APowerupBasic::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APowerupBasic, bIsActive);
}


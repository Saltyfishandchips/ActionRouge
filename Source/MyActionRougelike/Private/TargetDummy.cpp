// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDummy.h"
#include "AttributeComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATargetDummy::ATargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComp"));
	//触发OnHealthChanged事件，括号里是八股文，规定就是这么写的
	AttributeComp->OnHealthChanged.AddDynamic(this, &ATargetDummy::OnHealthChanged);
}

// Called when the game starts or when spawned
void ATargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATargetDummy::OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
	
}

// Called every frame
void ATargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


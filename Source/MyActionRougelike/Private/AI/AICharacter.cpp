// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "AIController.h"
#include "AttributeComponent.h"
#include "BrainComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "WorldUserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActionComponent.h"

// Sets default values
AAICharacter::AAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	AttributeComp = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComp"));

	ActionComp = CreateDefaultSubobject<UActionComponent>(TEXT("ActionComp"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//我们要实现AI被击中后产生推力，由于子弹Overlap最先的对象是胶囊体，因此我们需要取消其碰撞
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	//打开角色骨骼体的重合事件
	GetMesh()->SetGenerateOverlapEvents(true);

	TimeToHitParaName = "TimeToHit";
	TargetActorKey = "TargetActor";
}

void AAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AAICharacter::OnPawnSeen);

	AttributeComp->OnHealthChanged.AddDynamic(this, &AAICharacter::OnHealthChanged);
}


void AAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}

}

void AAICharacter::OnPawnSeen(APawn* Pawn)
{
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);

		MulticastPawnSeen();

	}


	/*DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SOPTTED", nullptr, FColor::White, 4.f, true);*/
	
}


void AAICharacter::MulticastPawnSeen_Implementation()
{
	UWorldUserWidget* NewWidget = CreateWidget<UWorldUserWidget>(GetWorld(), SpottedWidgetClass);

	if (NewWidget)
	{
		NewWidget->AttachedActor = this;

		NewWidget->AddToViewport(10);
	}
}



AActor* AAICharacter::GetTargetActor() const
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}

	return nullptr;
}


void AAICharacter::OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.f)
	{
		//确定AI并不是击中了自身
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		//检查AI是否有HealthBar
		if (InstigatorActor != this && ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}
		

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParaName, GetWorld()->TimeSeconds);

		if (NewHealth <= 0.f)
		{
			//停止行为树
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");

			}
			//RagDoll 布娃娃系统
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			//在击中后，构造函数中的设置将会使得胶囊体留在原地，因此我们需要解除胶囊体的碰撞
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			ActiveHealthBar->RemoveFromParent();

			//设置生命周期
			SetLifeSpan(3.f);
		}
	}
}


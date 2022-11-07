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

	//����Ҫʵ��AI�����к���������������ӵ�Overlap���ȵĶ����ǽ����壬���������Ҫȡ������ײ
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	//�򿪽�ɫ��������غ��¼�
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
		//ȷ��AI�����ǻ���������
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		//���AI�Ƿ���HealthBar
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
			//ֹͣ��Ϊ��
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");

			}
			//RagDoll ������ϵͳ
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			//�ڻ��к󣬹��캯���е����ý���ʹ�ý���������ԭ�أ����������Ҫ������������ײ
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			ActiveHealthBar->RemoveFromParent();

			//������������
			SetLifeSpan(3.f);
		}
	}
}


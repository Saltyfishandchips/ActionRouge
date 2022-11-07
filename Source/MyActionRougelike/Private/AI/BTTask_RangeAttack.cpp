// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RangeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AttributeComponent.h"


UBTTask_RangeAttack::UBTTask_RangeAttack()
{
	MaxBulletSpread = 2.0f;
}


EBTNodeResult::Type UBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* EmeneyAIController = OwnerComp.GetAIOwner();
	if (ensure(EmeneyAIController))
	{
		ACharacter* Emeney = Cast<ACharacter>(EmeneyAIController->GetPawn());
		if (Emeney == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = Emeney->GetMesh()->GetSocketLocation("Muzzle_01");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		//ȷ������Ƿ��Ѿ�����
		if (!UAttributeComponent::IsActorcAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}

		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		//��AI����ǹ�����ƫ��
		MuzzleRotation.Pitch += FMath::RandRange(0.f, MaxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		FTransform SpwanTM = FTransform(MuzzleRotation, MuzzleLocation);

		FActorSpawnParameters SpwanParams;
		SpwanParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//��Ҫ����Projectile�����������Լ���һ�࣬�ǳ���Ҫ
		SpwanParams.Instigator = Emeney;

		//SpawActor���ڵ�ǰ������ʹ�ø������ɽ�ɫ
		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjetileClass, SpwanTM, SpwanParams);

		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}

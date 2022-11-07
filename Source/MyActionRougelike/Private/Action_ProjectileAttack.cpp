// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_ProjectileAttack.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Math/RotationMatrix.h"
#include "MainCharacter.h"

UAction_ProjectileAttack::UAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}


void UAction_ProjectileAttack::StartAction_Implementation(AActor* Instigater)
{
	Super::StartAction_Implementation(Instigater);

	ACharacter* Player = Cast<ACharacter>(Instigater);
	
	if (Player)
	{
		Player->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Player->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		

		if (Player->HasAuthority())
		{
			FTimerHandle TimerHandle_AttackDelay;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "AttackDelay_Elapsed", Player);

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
		}
	}
}

void UAction_ProjectileAttack::AttackDelay_Elapsed(AMainCharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector GunLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		FActorSpawnParameters SpwanParams;
		SpwanParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpwanParams.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(20.f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		//这里会使用MainCharacter中重写的GetPawnViewLocation函数，用于获取Camera的位置
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();

		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000);

		FHitResult Hit;

		bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params);
		AActor* HitActor = Hit.GetActor();
		if (bBlockingHit)
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - TraceStart).Rotator();

		/*FTransform SpwanTM = FTransform(GetControlRotation(), GunLocation);*/
		FTransform SpwanTM = FTransform(ProjRotation, GunLocation);

		//SpawActor是在当前世界中使用该类生成角色
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpwanTM, SpwanParams);
	}

	StopAction(InstigatorCharacter);
}

 
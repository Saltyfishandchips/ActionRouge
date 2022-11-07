// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackBallBasic.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
AAttackBallBasic::AAttackBallBasic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->BodyInstance.SetCollisionProfileName("Projectile");
	//������������¼�
	SphereComp->OnComponentHit.AddDynamic(this, &AAttackBallBasic::OnHit);
	RootComponent = SphereComp;

	ImpactFX = CreateDefaultSubobject<UParticleSystem>(TEXT("ImpactFX"));

	//��ʼ����Ч���
	ImpactSound = CreateDefaultSubobject<UAudioComponent>(TEXT("MagicBallAudio"));
	ImpactSound->SetupAttachment(RootComponent);

	ProjectileAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("ProjectileAudio"));
	ProjectileAudio->SetupAttachment(RootComponent);

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->SetUpdatedComponent(SphereComp);
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;

	//�����������
	ImpactShakeInnerRadius = 0.f;
	ImpactShakeOuterRadius = 1500.f;

	//�������縴�ƣ��ڷ���˲���ʱ���ͻ���Ҳ���Ի�ȡ�ø���
	SetReplicates(true);
}


void AAttackBallBasic::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void AAttackBallBasic::Explode_Implementation()
{
	if (ensure(!IsPendingKill()))
	{
		//ֱ���ڻ�����ʹ�û��е�������Ч
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, GetActorLocation(), GetActorRotation());
		//����Ч��
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound->Sound, GetActorLocation());

		//�������
		UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
		Destroy();

		
	}
}
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
	//启动弹丸击中事件
	SphereComp->OnComponentHit.AddDynamic(this, &AAttackBallBasic::OnHit);
	RootComponent = SphereComp;

	ImpactFX = CreateDefaultSubobject<UParticleSystem>(TEXT("ImpactFX"));

	//初始化音效组件
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

	//相机抖动参数
	ImpactShakeInnerRadius = 0.f;
	ImpactShakeOuterRadius = 1500.f;

	//设置网络复制，在服务端产生时，客户端也可以获取该副本
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
		//直接在基类中使用击中的粒子特效
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, GetActorLocation(), GetActorRotation());
		//声音效果
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound->Sound, GetActorLocation());

		//相机抖动
		UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
		Destroy();

		
	}
}
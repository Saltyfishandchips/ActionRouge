// Fill out your copyright notice in the Description page of Project Settings.

#include "MagicBall.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "AttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AMagicBall::AMagicBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//设置根节点为碰撞体节点
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->BodyInstance.SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	//初始化粒子特效组件
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectComp"));
	EffectComp->SetupAttachment(RootComponent);

	HitCharacter = CreateDefaultSubobject<UParticleSystem>(TEXT("HitCharacter"));
	HitWorld = CreateDefaultSubobject<UParticleSystem>(TEXT("HitWorld"));

	//初始化音效组件
	MagicBallAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("MagicBallAudio"));
	MagicBallAudio->SetupAttachment(RootComponent);

	//初始化投射物物理组件
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->SetUpdatedComponent(SphereComp);
	MovementComp->InitialSpeed = 5000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;
	
	//初始化伤害
	DamageAmount = 20.f;

	// 组件击中某物时调用的事件。
	/*SphereComp->OnComponentHit.AddDynamic(this, &AMagicBall::OnHit);*/
	//当碰撞盒重合时，调用函数查询击中Actor的Attribute组件中的Health，并相应的减少Health
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMagicBall::OnActorOverlap);

	ImpactShakeInnerRadius = 0.f;
	ImpactShakeOuterRadius = 1500.f;
}

// Called when the game starts or when spawned
void AMagicBall::BeginPlay()
{
	Super::BeginPlay();
	//为子弹设置5秒的存在时间

	UGameplayStatics::PlaySoundAtLocation(this, MagicBallAudio->Sound, GetActorLocation());
	SetLifeSpan(5);
}

// Called every frame
void AMagicBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagicBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	
}

void AMagicBall::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		//GetComponentByClass迭代重叠的Actor中的所有组件直到找到我们所指的组件
		UAttributeComponent* AttributeComp = Cast<UAttributeComponent>(OtherActor->GetComponentByClass(UAttributeComponent::StaticClass()));
		if (AttributeComp)	//确保AttributeComp并不是空指针
		{
			AttributeComp->ApplyHealthChange(GetInstigator(), -DamageAmount);

			UGameplayStatics::PlayWorldCameraShake(this, CameraShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
			
			//击中AI或者敌人
			FTransform SpawnTransform = FTransform(OtherActor->GetActorRotation(), OtherActor->GetActorLocation());
			//添加子弹爆炸例子特效
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitCharacter, SpawnTransform, true, EPSCPoolMethod::None, true);
			Destroy();
		}

		//击中世界场景
		FTransform SpawnTransform = FTransform(OtherActor->GetActorRotation(), OtherActor->GetActorLocation());
		//添加子弹爆炸例子特效
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitWorld, SpawnTransform, true, EPSCPoolMethod::None, true);
	}
}

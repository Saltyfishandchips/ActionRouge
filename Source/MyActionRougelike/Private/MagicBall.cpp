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

	//���ø��ڵ�Ϊ��ײ��ڵ�
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->BodyInstance.SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	//��ʼ��������Ч���
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectComp"));
	EffectComp->SetupAttachment(RootComponent);

	HitCharacter = CreateDefaultSubobject<UParticleSystem>(TEXT("HitCharacter"));
	HitWorld = CreateDefaultSubobject<UParticleSystem>(TEXT("HitWorld"));

	//��ʼ����Ч���
	MagicBallAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("MagicBallAudio"));
	MagicBallAudio->SetupAttachment(RootComponent);

	//��ʼ��Ͷ�����������
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->SetUpdatedComponent(SphereComp);
	MovementComp->InitialSpeed = 5000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;
	
	//��ʼ���˺�
	DamageAmount = 20.f;

	// �������ĳ��ʱ���õ��¼���
	/*SphereComp->OnComponentHit.AddDynamic(this, &AMagicBall::OnHit);*/
	//����ײ���غ�ʱ�����ú�����ѯ����Actor��Attribute����е�Health������Ӧ�ļ���Health
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMagicBall::OnActorOverlap);

	ImpactShakeInnerRadius = 0.f;
	ImpactShakeOuterRadius = 1500.f;
}

// Called when the game starts or when spawned
void AMagicBall::BeginPlay()
{
	Super::BeginPlay();
	//Ϊ�ӵ�����5��Ĵ���ʱ��

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
		//GetComponentByClass�����ص���Actor�е��������ֱ���ҵ�������ָ�����
		UAttributeComponent* AttributeComp = Cast<UAttributeComponent>(OtherActor->GetComponentByClass(UAttributeComponent::StaticClass()));
		if (AttributeComp)	//ȷ��AttributeComp�����ǿ�ָ��
		{
			AttributeComp->ApplyHealthChange(GetInstigator(), -DamageAmount);

			UGameplayStatics::PlayWorldCameraShake(this, CameraShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
			
			//����AI���ߵ���
			FTransform SpawnTransform = FTransform(OtherActor->GetActorRotation(), OtherActor->GetActorLocation());
			//����ӵ���ը������Ч
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitCharacter, SpawnTransform, true, EPSCPoolMethod::None, true);
			Destroy();
		}

		//�������糡��
		FTransform SpawnTransform = FTransform(OtherActor->GetActorRotation(), OtherActor->GetActorLocation());
		//����ӵ���ը������Ч
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitWorld, SpawnTransform, true, EPSCPoolMethod::None, true);
	}
}

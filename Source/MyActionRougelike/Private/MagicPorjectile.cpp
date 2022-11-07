// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicPorjectile.h"
#include "AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GamePlayFunctionLibrary.h"
#include "ActionComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ActionEffect.h"

AMagicPorjectile::AMagicPorjectile()
{
	DamageAmount = 20.f;
}

void AMagicPorjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMagicPorjectile::OnActorOverlap);
}

void AMagicPorjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		//ʹ��PlayTagʱ�����ǲ�����Fnameһ��ֱ��ʹ�� =
		//FName Muzzle = "Muzzle_01";

		//������Ҫʹ��RequestGamePlayTag��ȡ���ƣ�����ڴ�������Ҫʹ��Ӳ���룬���Կ���ʹ��static������ʹ��Tag������һ�κ�ֱ�ӱ������ڴ���
		//static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Stats.Parring");

		//�����е�Actor�Ƿ����ض���GameTag������У��򽫵��跴��
		UActionComponent* ActionComp = Cast<UActionComponent>(OtherActor->GetComponentByClass(UActionComponent::StaticClass()));
		
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;

			//��ת�ӵ��󣬽�������Ϊ�µ�Instigator����
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		if (UGamePlayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();

			if (ActionComp && HasAuthority())
			{
				AActor* Ins = GetInstigator();
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
		}
	}
}

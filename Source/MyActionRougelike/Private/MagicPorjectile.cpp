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
		//使用PlayTag时，我们不能像Fname一样直接使用 =
		//FName Muzzle = "Muzzle_01";

		//而是需要使用RequestGamePlayTag获取名称，如果在代码中需要使用硬编码，可以考虑使用static，可以使得Tag在运行一次后直接保存在内存中
		//static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Stats.Parring");

		//检测击中的Actor是否含有特定的GameTag，如果有，则将弹丸反弹
		UActionComponent* ActionComp = Cast<UActionComponent>(OtherActor->GetComponentByClass(UActionComponent::StaticClass()));
		
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;

			//反转子弹后，将敌人作为新的Instigator传入
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

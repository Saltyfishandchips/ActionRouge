// Fill out your copyright notice in the Description page of Project Settings.

#include "AttributeComponent.h"
#include "Math/UnrealMathUtility.h"
#include "ActionGameModeBase.h"
#include "HAL/IConsoleManager.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.f, TEXT("Global Damage Modifer for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;

	RageMax = 100;
	Rage = 0.f;

	//在Components中使用网络复制需要使用SetIsReplicatedByDefault()
	SetIsReplicatedByDefault(true);
}

bool UAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

bool UAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

bool UAttributeComponent::IsFullHealth()
{
	return Health == HealthMax;
}


float UAttributeComponent::GetHealth()
{
	return Health;
}

float UAttributeComponent::GetHealthMax()
{
	return HealthMax;
}


void UAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}


bool UAttributeComponent::ApplyHealthChange(AActor* InstigatoActor, float Delta)
{
	//开启上帝模式，通过CanBeDamaged控制Attribute组件是否能够改变HP
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	
	//当发生伤害时，查询CVarDmageMultiplier是否有值
	if (Delta< 0.f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;

	//只有在服务端更新HP
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		//如果生命值已经为0，则不需要再进行广播，在多人联机的编程中，需要尽可能减少不需要的信息开支
		//只有服务端运行组播
		if (ActualDelta != 0.f)
		{
			MulticastHealthChanged(InstigatoActor, Health, ActualDelta);
		}

		//死亡
		if (ActualDelta < 0.0f && Health == 0.0f)
		{
			//获取GamMode，由于该函数是模板函数，因此可以不使用Cast
			AActionGameModeBase* GM = GetWorld()->GetAuthGameMode<AActionGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatoActor);
			}
		}
	}

	/*OnHealthChanged.Broadcast(InstigatoActor, this, Health, ActualDelta);*/

	return ActualDelta != 0;
}

UAttributeComponent* UAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UAttributeComponent>(FromActor->GetComponentByClass(UAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UAttributeComponent::IsActorcAlive(AActor* Actor)
{
	UAttributeComponent* Attributes = GetAttributes(Actor);
	if (Attributes)
	{
		return Attributes->IsAlive();
	}
	return false;
}


float UAttributeComponent::GetRage()
{
	return Rage;
}

bool UAttributeComponent::IsFullRage()
{
	return Rage == RageMax;
}

bool UAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{

	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.f, RageMax);

	float ActualDelta = Rage - OldRage;

	if (ActualDelta != 0.f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
	}


	return ActualDelta != 0;
}


void UAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}




void UAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAttributeComponent, Health);

	DOREPLIFETIME(UAttributeComponent, HealthMax);

	DOREPLIFETIME(UAttributeComponent, Rage);

	DOREPLIFETIME(UAttributeComponent, RageMax);
	//网络优化，只发送必要的数据
	//HealthMax在变化时只有Owner和Sever能看到变化
	/*DOREPLIFETIME_CONDITION(UAttributeComponent, HealthMax, COND_OwnerOnly);*/
}


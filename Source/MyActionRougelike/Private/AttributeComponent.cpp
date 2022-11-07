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

	//��Components��ʹ�����縴����Ҫʹ��SetIsReplicatedByDefault()
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
	//�����ϵ�ģʽ��ͨ��CanBeDamaged����Attribute����Ƿ��ܹ��ı�HP
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	
	//�������˺�ʱ����ѯCVarDmageMultiplier�Ƿ���ֵ
	if (Delta< 0.f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;

	//ֻ���ڷ���˸���HP
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		//�������ֵ�Ѿ�Ϊ0������Ҫ�ٽ��й㲥���ڶ��������ı���У���Ҫ�����ܼ��ٲ���Ҫ����Ϣ��֧
		//ֻ�з���������鲥
		if (ActualDelta != 0.f)
		{
			MulticastHealthChanged(InstigatoActor, Health, ActualDelta);
		}

		//����
		if (ActualDelta < 0.0f && Health == 0.0f)
		{
			//��ȡGamMode�����ڸú�����ģ�庯������˿��Բ�ʹ��Cast
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
	//�����Ż���ֻ���ͱ�Ҫ������
	//HealthMax�ڱ仯ʱֻ��Owner��Sever�ܿ����仯
	/*DOREPLIFETIME_CONDITION(UAttributeComponent, HealthMax, COND_OwnerOnly);*/
}


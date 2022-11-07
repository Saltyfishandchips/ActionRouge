// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEffect.h"
#include "ActionComponent.h"
#include "GameFramework/GameStateBase.h"

UActionEffect::UActionEffect()
{
	bAutoStart = true;
}



void UActionEffect::StartAction_Implementation(AActor* Instigater)
{
	Super::StartAction_Implementation(Instigater);
	
	if (Duration > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigater);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigater);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, false);
	}

}

void UActionEffect::StopAction_Implementation(AActor* Instigater)
{
	//������Super�ĸ����������Ƴ�GameTag�Ĳ��������ǲ����Buff�Ƿ���Ҫ����Tag����������жϷ���Superǰ��
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigater);
	}

	Super::StopAction_Implementation(Instigater);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	UActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(this);
	}
}


float UActionEffect::GetTimeRemaining() const
{
	AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();

	if (GS)
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}

	return Duration;
	//return EndTime - GetWorld()->TimeSeconds;
}



void UActionEffect::ExecutePeriodicEffect_Implementation(AActor* instigator)
{

}
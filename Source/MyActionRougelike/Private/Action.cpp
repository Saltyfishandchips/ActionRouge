// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"
#include "Logging/LogMacros.h"
#include "ActionComponent.h"
#include <../MyActionRougelike.h>
#include "Net/UnrealNetwork.h"

void UAction::Initialize(UActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}


bool UAction::CanStart_Implementation(AActor* instigater)
{
	if (IsRunning())
	{
		return false;
	}

	UActionComponent* Comp = GetOwningComponent();

	//检查GameTag中是否有BlockTags，如果有则返回false
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void UAction::StartAction_Implementation(AActor* Instigater)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigater;

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}


	GetOwningComponent()->OnACtionStarted.Broadcast(GetOwningComponent(), this);
}

void UAction::StopAction_Implementation(AActor* Instigater)
{
	UE_LOG(LogTemp, Log, TEXT("Stoped: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::Red);

	//ensureAlways(bIsRunning);

	UActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigater;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}


UWorld* UAction::GetWorld() const
{
	//Outer is set when creating action via NewObject<T>
	//Level下的所有物体都是Actor，Action可以作为一种存储技能的相关信息和逻辑的游戏数据而存储，而不一定要存在于世界中
	//Action一定是挂载在角色的ActorComponent上，因此我们重写GetWorld，可以确保Action来自Component
	/*UActorComponent* Comp = Cast<UActorComponent>(GetOuter());*/
	AActor* Actor = Cast<AActor>(GetOuter());

	//if (Comp)
	//{
	//	return Comp->GetWorld();
	//}

	//UE将Action进行网络复制时，规定Outer不可以时Component，而应该是Actor
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

UActionComponent* UAction::GetOwningComponent() const
{
	//方法一，需要不停迭代，不适合
	//AActor* Actor = Cast<AActor>(GetOuter());
	//return Actor->GetComponentByClass(UActionComponent::StaticClass());

	//return Cast<UActionComponent>(GetOuter());
	return ActionComp;
}

void UAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}


bool UAction::IsRunning() const
{
	return RepData.bIsRunning;
}




void UAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAction, RepData);
	DOREPLIFETIME(UAction, ActionComp);
	DOREPLIFETIME(UAction, TimeStarted);
}



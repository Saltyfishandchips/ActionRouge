// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionComponent.h"
#include "Action.h"
#include "Engine/Engine.h"
#include "../MyActionRougelike.h"
#include "Engine/ActorChannel.h"

UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


void UActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//BeginPlayͬʱ�������ڷ���˺Ϳͻ���
	//�����Action����ֱ���ڿͻ���������

	//����ϣ��GAS��Actionֻ�ڷ����������У�Ȼ�����н�����Ƶ��ͻ���

	//ֻ������Server��
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}


void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOuter()) + ": " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugMsg);

	//��������Action������Ϣ��ӡ����
	//for (UAction* Action : Actions)
	//{
	//	FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

	//	FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"),*GetNameSafe(GetOwner()), *GetNameSafe(Action));

	//	LogOnScreen(this, ActionMsg, TextColor, 0.f);

	//}
}



void UActionComponent::AddAction(AActor* Instigater, TSubclassOf<UAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	UAction* NewAction = NewObject<UAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);

		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigater)))
		{
			NewAction->StartAction(Instigater);
		}
	}

}


void UActionComponent::RemoveAction(UAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

bool UActionComponent::StartActionByName_Implementation(AActor* Instigater, FName ActionName)
{
	for (UAction* Action: Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigater))
			{
				FString FaildMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FaildMsg);
				continue;
			}

			//����Ƿ�Ϊ�ͻ���
			if (!GetOwner()->HasAuthority())
			{
				//����ǿͻ��ˣ�ֱ�ӷ���SeverRPC
				ServerStartAction(Instigater, ActionName);
			}

			Action->StartAction(Instigater);
			return true;
		}
	}
	return false;
}


bool UActionComponent::StopActionByName_Implementation(AActor* Instigater, FName ActionName)
{
	for (UAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{

				//����Ƿ�Ϊ�ͻ���
				if (!GetOwner()->HasAuthority())
				{
					//����ǿͻ��ˣ�ֱ�ӷ���SeverRPC
					ServerStopAction(Instigater, ActionName);
				}

				Action->StopAction(Instigater);
				return true;
			}
		}
	}
	return false;
}

void UActionComponent::ServerStartAction_Implementation(AActor* Instigater, FName ActionName)
{
	StartActionByName(Instigater, ActionName);
}

void UActionComponent::ServerStopAction_Implementation(AActor* Instigater, FName ActionName)
{
	StopActionByName(Instigater, ActionName);
}



UAction* UActionComponent::GetAction(TSubclassOf<UAction> ActionClass) const
{
	for (UAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}

bool UActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSometing = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UAction* Action : Actions)
	{
		if (Action)
		{
			WroteSometing |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSometing;
}


void UActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UActionComponent, Actions);
}

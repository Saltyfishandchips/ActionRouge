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
	
	//BeginPlay同时能运行在服务端和客户端
	//因此在Action可以直接在客户端中运行

	//我们希望GAS的Action只在服务器端运行，然后将运行结果复制到客户端

	//只运行在Server上
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

	//遍历所有Action，将信息打印出来
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

			//检测是否为客户端
			if (!GetOwner()->HasAuthority())
			{
				//如果是客户端，直接发送SeverRPC
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

				//检测是否为客户端
				if (!GetOwner()->HasAuthority())
				{
					//如果是客户端，直接发送SeverRPC
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

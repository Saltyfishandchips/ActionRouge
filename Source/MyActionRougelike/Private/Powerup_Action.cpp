// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerup_Action.h"
#include <ActionComponent.h>
#include "Engine/Engine.h"

void APowerup_Action::Interaction_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	UActionComponent* ActionComp = Cast<UActionComponent>(InstigatorPawn->GetComponentByClass(UActionComponent::StaticClass()));

	if (ActionComp)
	{
		if (ActionComp->GetAction(ActionToGrant))
		{
			//UE_LOG(LogTemp, Log, TEXT("Instigator already has action of class: %s"), *GetNameSafe(ActionToGrant));
			FString DebugMsg = FString::Printf(TEXT("Action '%s' already known."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return;
		}

		ActionComp->AddAction(InstigatorPawn, ActionToGrant);
		HideAndCooldownPowerup();
	}
}

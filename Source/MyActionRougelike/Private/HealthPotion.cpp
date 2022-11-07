// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPotion.h"
#include "AttributeComponent.h"
#include "MainPlayerState.h"

AHealthPotion::AHealthPotion()
{
	CreditCost = 50;
}


void AHealthPotion::Interaction_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(InstigatorPawn);

	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (AMainPlayerState* PS = InstigatorPawn->GetPlayerState<AMainPlayerState>())
		{
			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
			{
				HideAndCooldownPowerup();
			}
		}
	}
}

FText AHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		return NSLOCTEXT("InteractableActors", "HealhPotion_FullHealthWarning", "Health is already maximum");
	}

	return FText::Format(NSLOCTEXT("InteractableActors", "HealhPotion_InteractMessage", "Cost {0} Credits. Restors health to maximum."), CreditCost);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerup_CreditCoin.h"
#include "MainPlayerState.h"


APowerup_CreditCoin::APowerup_CreditCoin()
{
	CreditCost = 80;
}


void APowerup_CreditCoin::Interaction_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (AMainPlayerState* PS = InstigatorPawn->GetPlayerState<AMainPlayerState>())
	{
		PS->AddCredits(CreditCost);
		HideAndCooldownPowerup();
	}

}

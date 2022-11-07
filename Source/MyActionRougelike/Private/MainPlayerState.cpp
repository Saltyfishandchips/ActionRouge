// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerState.h"
#include "PlayerSaveGame.h"



int32 AMainPlayerState::GetCredits() const
{
	return Credits;
}

void AMainPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0.f))
	{
		return;
	}

	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool AMainPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0.f))
	{
		return false;
	}

	if (Credits < Delta)
	{
		return false;
	}

	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, -Delta);

	return true;
}


void AMainPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}




void AMainPlayerState::SavePlayerState_Implementation(UPlayerSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}

void AMainPlayerState::LoadPlayerState_Implementation(UPlayerSaveGame* SaveObject)
{
	if (SaveObject)
	{
		//Credits = SaveObject->Credits;
		AddCredits(SaveObject->Credits);
	}
}


void AMainPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMainPlayerState, Credits)
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AMainPlayerState*, Playerstate, int32, NewCredits, int32, Delta);

class UPlayerSaveGame;

/**
 * 
 */
UCLASS()
class MYACTIONROUGELIKE_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(ReplicatedUsing = "OnRep_Credits", EditDefaultsOnly, Category = "Credits")
	int32 Credits;

	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);

public:

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(UPlayerSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(UPlayerSaveGame* SaveObject);
};

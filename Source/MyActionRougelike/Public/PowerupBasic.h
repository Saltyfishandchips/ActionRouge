// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "TimerManager.h"
#include "PowerupBasic.generated.h"

class USphereComponent;
class UStaticMeshComponent;


UCLASS()
class MYACTIONROUGELIKE_API APowerupBasic : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerupBasic();

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;

	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();

	FTimerHandle TimerHandle_RespawnTimer;

	UFUNCTION()
	void ShowPowerup();

	UFUNCTION()
	void HideAndCooldownPowerup();

	UFUNCTION()
	void SetPowerupState(bool bNewIsActive);

public:
	void Interaction_Implementation(APawn* InstigatorPawn) override;


	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

};

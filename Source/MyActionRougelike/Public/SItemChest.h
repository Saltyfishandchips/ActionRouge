// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class MYACTIONROUGELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:

	//ReplicatedUsing会让UE在bLidOpen这一值改变时调用OnRep_OpenLid()函数，ReplicatedUsing在蓝图中的名称为RepNoeify
	//ReplicatedUsing在服务器端不会自动调用，需要手动调用，但是在蓝图中的RPC会自动调用.
	UPROPERTY(ReplicatedUsing = "OnRep_OpenLid", BlueprintReadOnly, SaveGame)
	bool bLidOpen;

	UFUNCTION()
	void OnRep_OpenLid();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

public:
	UPROPERTY(EditAnywhere)
	float TargetPitch;

	void Interaction_Implementation(APawn* InstigatorPawn) override;


	void OnActorLoaded_Implementation() override;

};

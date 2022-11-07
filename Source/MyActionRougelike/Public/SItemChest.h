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

	//ReplicatedUsing����UE��bLidOpen��һֵ�ı�ʱ����OnRep_OpenLid()������ReplicatedUsing����ͼ�е�����ΪRepNoeify
	//ReplicatedUsing�ڷ������˲����Զ����ã���Ҫ�ֶ����ã���������ͼ�е�RPC���Զ�����.
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

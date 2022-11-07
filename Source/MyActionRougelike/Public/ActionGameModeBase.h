// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/DataTable.h"
#include "ActionGameModeBase.generated.h"

class UPlayerSaveGame;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class UDataTable;
class UMonsterData;

USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FPrimaryAssetId MonsterID;
	//UMonsterData* MonsterData;
	//TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;
};


/**
 * 
 */
UCLASS()
class MYACTIONROUGELIKE_API AActionGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	FString SlotName;

	UPROPERTY()
	UPlayerSaveGame* CurrentSaveGame;

	//ɱ��AI���ܻ�õ�Credits
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	int32 CreditsPerKil;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;

	//UPROPERTY(EditDefaultsOnly, Category = "AI")
	//TSubclassOf<AActor> MinionClass;

	//GameMode�е�EQS��ѯ�����ڲ�ѯָ��λ�ã��Ա�֮������AI
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	//AI�����������Ѷ�����
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	//����AI��EQS��ʱ��ģ��
	FTimerHandle TimerHandle_SpawnBots;

	//AI���ɼ������ʼΪ2s
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	//AI���ɺ���
	UFUNCTION()
	void SpawnBotTimerElapsed();

	//EQS���
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnMonsterLoaded(FPrimaryAssetId LoadedID, FVector SpawnLocation);

	UFUNCTION()
	//�������
	void RespawnPlayerElapsed(AController* Controller);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	FVector WorldOffSet;

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	float DesiredPowerupCount;

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	float RequiredPowerupDistance;
	
	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	TArray<TSubclassOf<AActor>> CoinClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	UEnvQuery* SpawnCreditsCoinQuery;

	//����Ӳ�ҵĶ�ʱʱ��
	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	float SpawnCoinInterval;

	UFUNCTION()
	void OnQueryCreditsCoinCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

public:
	
	UFUNCTION()
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	AActionGameModeBase();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual void StartPlay() override;

	//����̨������ɱ������AI
	UFUNCTION(Exec)
	void KillallAI();


	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();
};

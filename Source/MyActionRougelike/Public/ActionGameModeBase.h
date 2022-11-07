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

	//杀死AI所能获得的Credits
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	int32 CreditsPerKil;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;

	//UPROPERTY(EditDefaultsOnly, Category = "AI")
	//TSubclassOf<AActor> MinionClass;

	//GameMode中的EQS查询，用于查询指定位置，以便之后生成AI
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	//AI生成数量的难度曲线
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	//生成AI的EQS定时器模块
	FTimerHandle TimerHandle_SpawnBots;

	//AI生成间隔，初始为2s
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	//AI生成函数
	UFUNCTION()
	void SpawnBotTimerElapsed();

	//EQS完成
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnMonsterLoaded(FPrimaryAssetId LoadedID, FVector SpawnLocation);

	UFUNCTION()
	//玩家重生
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

	//积分硬币的定时时间
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

	//控制台函数，杀死所有AI
	UFUNCTION(Exec)
	void KillallAI();


	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();
};

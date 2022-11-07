// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AttributeComponent.h"
#include "AI/AICharacter.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "MainCharacter.h"
#include "HAL/IConsoleManager.h"
#include "MainPlayerState.h"
#include "PlayerSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework\GameStateBase.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Serialization/MemoryReader.h"
#include "MonsterData.h"
#include "ActionComponent.h"
#include "Engine\AssetManager.h"

static TAutoConsoleVariable<bool> CVarSpwanBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

AActionGameModeBase::AActionGameModeBase()
{
	SpawnTimerInterval = 2.0f;

	SpawnCoinInterval = 2.f;
	DesiredPowerupCount = 5;
	RequiredPowerupDistance = 2000;


	CreditsPerKil = 20;

	PlayerStateClass = AMainPlayerState::StaticClass();
	
	SlotName = "SaveGame01";
}

void AActionGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void AActionGameModeBase::StartPlay()
{
	Super::StartPlay();

	//生成AI的定时器
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AActionGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	// Make sure we have assigned at least one power-up class
	if (ensure(CoinClasses.Num() > 0))
	{
		// Run EQS to find potential power-up spawn locations
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnCreditsCoinQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AActionGameModeBase::OnQueryCreditsCoinCompleted);
		}
	}
}

//命令台控制清除所有AI
void AActionGameModeBase::KillallAI()
{
	for (TActorIterator<AAICharacter> It(GetWorld()); It; ++It)
	{
		AAICharacter* Bot = *It;

		/*UAttributeComponent* AttributeComp = Cast<UAttributeComponent>(Bot->GetComponentByClass(UAttributeComponent::StaticClass()));*/
		UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(Bot);
		if (AttributeComp && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); // pass in player? for kill credit.
		}
	}
}



void AActionGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpwanBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("Bot spawning disabled via car cvar 'CVarSpawnBots'."));
		return;
	}
	

	//查询当前世界中NPC的数量
	int32 NrOfAliveBots = 0;
	for (TActorIterator<AAICharacter> It(GetWorld()); It; ++It)
	{
		AAICharacter* Bot = *It;

		/*UAttributeComponent* AttributeComp = Cast<UAttributeComponent>(Bot->GetComponentByClass(UAttributeComponent::StaticClass()));*/
		UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(Bot);
		if (AttributeComp && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots"), NrOfAliveBots);

	float MaxBotCount = 10.0f;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity.Skipping bot spawn."));
		return;
	}

	//如果NPC数量不足，则通过EQS查询生成位置
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AActionGameModeBase::OnQueryCompleted);
	}
	
}

//生成AI的EQS查询函数
void AActionGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{

 	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);

			//获取随机敌人
			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelcetedRow = Rows[RandomIndex];
			

			UAssetManager* Manager = UAssetManager::GetIfValid();
			if (Manager)
			{
				TArray<FName> Bundles;

				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &AActionGameModeBase::OnMonsterLoaded, SelcetedRow->MonsterID, Locations[0]);

				Manager->LoadPrimaryAsset(SelcetedRow->MonsterID, Bundles, Delegate);
			}

		}

		//GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		////画出所有NPC生成的位置
		//DrawDebugSphere(GetWorld(), Locations[0], 50.f, 20, FColor::Blue, false, 60.f);
	}
}


void AActionGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedID, FVector SpawnLocation)
{
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		UMonsterData* MonsterData = Cast<UMonsterData>(Manager->GetPrimaryAssetObject(LoadedID));

		if (MonsterData)
		{
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MinionClass, SpawnLocation, FRotator::ZeroRotator);
			if (NewBot)
			{
				LogOnScreen(this, FString::Printf(TEXT("Spawned emeny: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));

				UActionComponent* ActionComp = Cast<UActionComponent>(NewBot->GetComponentByClass(UActionComponent::StaticClass()));
				if (ActionComp)
				{
					for (TSubclassOf<UAction> ActionClass : MonsterData->Actions)
					{
						ActionComp->AddAction(NewBot, ActionClass);
					}
				}

			}
		}
	}
}

void AActionGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	UE_LOG(LogTemp, Log, TEXT("OnAcorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));

	AMainCharacter* Player = Cast<AMainCharacter>(VictimActor);
	if (Player)
	{
		//这里的计时器句柄一定得是局部变量
		//如果是全局变量，当多人游戏时，第二个玩家死亡后，该定时器将会覆盖第一个定时器，使得第一个玩家永远不会重生
		FTimerHandle Timer_RespawnDelay;

		//设置Timer中所需要的参数
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		//重生时间
		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(Timer_RespawnDelay, Delegate, RespawnDelay, false);
	}
	

	//检查有击杀的Actor是玩家的Pawn，由于只有玩家拥有AMainPlayerState，而AI没有AMainPlayerState，因此可以通过if判断
	//增加相应的击杀分数
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		AMainPlayerState* PS = KillerPawn->GetPlayerState<AMainPlayerState>();
		if (PS)
		{
			PS->AddCredits(CreditsPerKil);
		}
	}
}


void AActionGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		//解除控制器与player的连接
		//使用该函数是为了之后RestartPlayer中会检查Controller与pawn是否有联系，如果有则会直接获取pawn的Rotation
		//而我们需要的是直接copy一个新的pawn，因此需要先解除连接
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}



void AActionGameModeBase::OnQueryCreditsCoinCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	// Keep used locations to easily check distance between points
	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	// Break out if we reached the desired count or if we have no more potential positions remaining
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		// Pick a random location from remaining points.
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLocation = Locations[RandomLocationIndex];
		// Remove to avoid picking again
		Locations.RemoveAt(RandomLocationIndex);

		// Check minimum distance requirement
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPowerupDistance)
			{
				// Show skipped locations due to distance
				//DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 20, FColor::Red, false, 10.0f);

				// too close, skip to next attempt
				bValidLocation = false;
				break;
			}
		}

		// Failed the distance test
		if (!bValidLocation)
		{
			continue;
		}

		// Pick a random powerup-class
		int32 RandomClassIndex = FMath::RandRange(0, CoinClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = CoinClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation + WorldOffSet, FRotator::ZeroRotator);

		// Keep for distance checks
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}


void AActionGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{	
	AMainPlayerState* PS = NewPlayer->GetPlayerState<AMainPlayerState>();

	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}


void AActionGameModeBase::WriteSaveGame()
{
	//迭代所有的player state
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AMainPlayerState* PS = Cast<AMainPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;
		}
	}
	 
	//保存前清空
	CurrentSaveGame->SavedActors.Empty();

	//在整个GetWorld中迭代所有Actor
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;

		if (!Actor->Implements<USGameplayInterface>()) 
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetTransform();

		///
		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		//只寻找声明为UPROPERTY(SaveGame)的变量
		Ar.ArIsSaveGame = true;

		Actor->Serialize(Ar);


		CurrentSaveGame->SavedActors.Add(ActorData);
	}


	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}


void AActionGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));


		//在整个GetWorld中迭代所有Actor
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;

			if (!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData: CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					//只寻找声明为UPROPERTY(SaveGame)的变量
					Ar.ArIsSaveGame = true;

					Actor->Serialize(Ar);

					CurrentSaveGame->SavedActors.Add(ActorData);

					ISGameplayInterface::Execute_OnActorLoaded(Actor);


					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<UPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass()));
		
		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}



}

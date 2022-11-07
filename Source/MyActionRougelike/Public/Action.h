// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "Action.generated.h"

class UWorld;
class UTexture2D;

USTRUCT()
struct FActionRepData 
{
	GENERATED_BODY()

public:

	//在结构体中，网络复制的编写需要反转，当我们不需要复制时，使用NotReplicated
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;

};


/**
 * 
 */
//这里需要加上Blueprintable来告诉UE4
UCLASS(Blueprintable)
class MYACTIONROUGELIKE_API UAction : public UObject
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(Replicated)
	UActionComponent* ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;
	//bool bIsRunning;

	UPROPERTY(Replicated)
	float TimeStarted;

	UFUNCTION()
	void OnRep_RepData();


public:

	UFUNCTION()
	void Initialize(UActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* instigater);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigater);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigater);

 

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UWorld* GetWorld() const override;
	
	bool IsSupportedForNetworking() const override
	{
		return true;
	}

};

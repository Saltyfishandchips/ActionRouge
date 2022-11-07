// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "AICharacter.generated.h"

class UPawnSensingComponent;
class UAttributeComponent;
class UUserWidget;
class UWorldUserWidget;
class UActionComponent;

UCLASS()
class MYACTIONROUGELIKE_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAICharacter();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;

	/* AI�ڰ� 'TargetActor'�������� */
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TargetActorKey;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Effect")
	FName TimeToHitParaName;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	//���Actor���������HP��ֵ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UActionComponent* ActionComp;

	//AI��HP�ı亯��
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPawnSeen();

	UFUNCTION()
	void SetTargetActor(AActor* NewTarget);

	virtual void PostInitializeComponents() override;
};

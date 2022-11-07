// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

//AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta
//定义我们自己的事件，如同BeginOverlap之类的一样，首先需要通过UE的宏进行定义
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, UAttributeComponent*, OwningComp, float, NewAttributeValue, float, Delta);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYACTIONROUGELIKE_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorcAlive(AActor* Actor);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Arrtibute")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Arrtibute")
	float HealthMax;

	/*UPROPERTY(ReplicatedUsing = "OnRep_IsAlive")
	bool bIsAlive;*/

	//ReplicatedUsing用于通知客户端中Actor状态的变化，例如死亡
	//而NetMulticast更多用于瞬间事态的变化，例如油桶爆炸，击中目标，但不一定会改变Actor的状态

	//这里可以使用UnReliable，因为我们将Health设置为了Replicated,在客户端中我们无论如何都可以得到Health值
	//但是在MainCharacter中，HealthChanged将会影响Controller的输入，因此我们在这设置成为Reliable。
	//在Server中调用MultiCast将会给自己和其他所有客户端调用，Client调用则就是在本地调用，不会给其他玩家调用

	//此外，在大型网游中，还需要考虑Actor的相关性，例如在大逃杀中，地图边缘的箱子被开启
	//而角色如果在地图另一边，则角色不需要直到该箱子的状态，直到其离得足够近
	//如果使用NetMulticast和Reliable，当我的属性改变时，该关卡内的所有其他玩家都将会获取到属性的变化，这会造成网络的浪费

	UFUNCTION(NetMulticast, Reliable)//@FIXME:将MainCharacter中的死亡移除后，‘state’移出MainCharacter后，将其设置为UnReliable
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);


	//怒气值
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Arrtibute")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Arrtibute")
	float RageMax;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);

public:

	/*---------------------
		Health下的各个函数改变
	----------------------*/
	UFUNCTION(BlueprintCallable)
	float GetHealth();

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth();

	UFUNCTION(BlueprintCallable)
	float GetHealthMax();

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool ApplyHealthChange(AActor* InstigatoActor, float Delta);
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;


	UFUNCTION(BlueprintCallable)
	float GetRage();

	UFUNCTION(BlueprintCallable)
	bool IsFullRage();

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;
};
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "GameplayTagContainer.h"
#include <ActionComponent.h>

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwingActor = GetOwningActor();
	if (OwingActor)
	{
		ActionComp = Cast<UActionComponent>(OwingActor->GetComponentByClass(UActionComponent::StaticClass()));
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");

	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}

}

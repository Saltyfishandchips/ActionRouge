// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEffect_Thorns.h"
#include "AttributeComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GamePlayFunctionLibrary.h"

UActionEffect_Thorns::UActionEffect_Thorns()
{
	RefectFraction = 0.2f;

	Duration = 0.f;
	Period = 0.f;
}


void UActionEffect_Thorns::StartAction_Implementation(AActor* Instigater)
{
	Super::StartAction_Implementation(Instigater);

	UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &UActionEffect_Thorns::OnHealthChanged);
	}
}

void UActionEffect_Thorns::StopAction_Implementation(AActor* Instigater)
{
	Super::StartAction_Implementation(Instigater);

	UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());

	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &UActionEffect_Thorns::OnHealthChanged);
	}
}

void UActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewAttributeValue, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	if (Delta < 0.f && OwningActor != InstigatorActor)
	{

		int32 ReflectedAmount = FMath::RoundToInt(Delta * RefectFraction);
		if (RefectFraction == 0)
		{
			return;
		}

		ReflectedAmount = FMath::Abs(ReflectedAmount);

		UGamePlayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}

}
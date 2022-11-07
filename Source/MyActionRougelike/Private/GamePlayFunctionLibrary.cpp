// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayFunctionLibrary.h"
#include "AttributeComponent.h"

bool UGamePlayFunctionLibrary::ApplyDamage(AActor* DamageActor, AActor* TargetActor, float DamageAmount)
{
	UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageActor, -DamageAmount);
	}
	return false;
}

bool UGamePlayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			//// Direction = Target - Origin
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();


			HitComp->AddImpulseAtLocation(Direction * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}

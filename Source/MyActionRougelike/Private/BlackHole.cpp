// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AttributeComponent.h"

ABlackHole::ABlackHole() {

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(RootComponent);
	//要设置自动打开，否则不会触发径向力组件
	RadialForce->SetAutoActivate(true);
	RadialForce->Radius = 1000.f;
	RadialForce->ImpulseStrength = 1000.f;
	RadialForce->ForceStrength = -2000000.f;
	RadialForce->bImpulseVelChange = true;
	RadialForce->AddCollisionChannelToAffect(ECC_PhysicsBody);
	RadialForce->AddCollisionChannelToAffect(ECC_WorldDynamic);
	RadialForce->AddCollisionChannelToAffect(ECC_Destructible);
	

	SphereComp->SetSphereRadius(300.f);
	SphereComp->SetCollisionProfileName("BlackHole");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::BlackHoleEffect);

	LifeTime = 5.f;
}

void ABlackHole::BlackHoleEffect(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 num, bool thing, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(OtherActor);
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(GetInstigator(), -10.f);
		}
		

		/*OtherActor->Destroy();*/
	}
}

void ABlackHole::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetLifeSpan(LifeTime);
}



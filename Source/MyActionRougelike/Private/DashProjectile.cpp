// Fill out your copyright notice in the Description page of Project Settings.


#include "DashProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "AttackBallBasic.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h"

ADashProjectile::ADashProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;
	MovementComp->InitialSpeed = 5000.f;
	
}

void ADashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ADashProjectile::Explode, DetonateDelay);
}

void ADashProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	//Ìí¼ÓÒôÐ§
	UGameplayStatics::PlaySoundAtLocation(this, ProjectileAudio->Sound, GetActorLocation());

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, GetActorLocation(), GetActorRotation());

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ADashProjectile::TeleportInstigator, TeleportDelay);

}

void ADashProjectile::TeleportInstigator()
{
	AActor* ActortoInstigator = GetInstigator();
	if (ActortoInstigator)
	{
		ActortoInstigator->TeleportTo(GetActorLocation(), GetActorRotation());
	}
	Destroy();
}

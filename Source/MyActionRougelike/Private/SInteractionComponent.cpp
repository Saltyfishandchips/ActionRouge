// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "SInteractionComponent.h"
#include "CollisionQueryParams.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/GameplayStatics.h"
#include <WorldUserWidget.h>


//����̨�����Ƿ�������Debug����
static TAutoConsoleVariable<bool> CVarDebugDrawInteract(TEXT("su.DebugDrawInteract"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 500.f;
	TranceRadius = 30.f;
	CollisionChannel = ECC_WorldDynamic;
}


void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	//Tick�����������ڷ���˻��ǿͻ��˶������У��������ͨ�����ؼ�����������߼�⣬ֻ�е�����ʹ��Controllerʱ�������߼��
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
	
}


void USInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteract.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	//�����ײ��ѯ������
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	//��������������ʱ��GetActorEyesViewPoint���Ի�ȡPawn���۲�λ�ú���ת������ֵ������ȥ��ʵ��
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	//���ǽ�End����Ϊģ�͵��۲�λ�ü��Ͽ���������ĵط�����500
	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	TArray<FHitResult> Hits;


	FCollisionShape Shape;
	Shape.SetSphere(TranceRadius);


	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	//���FocusedActor
	FocusedActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TranceRadius, 32, LineColor, false, 2.0f);
		}

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			//�����ײ�������ǲ��ǿɽ����ģ���Ҫע��ʹ��ǰ׺U���࣬��Ҫʹ��ǰ׺I
			if (HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}


	//GetActor()�����ر����е�Actor��
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	}
}

void USInteractionComponent::PrimaryInteraction()
{
	//if (FocusedActor == nullptr)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
	//	return;
	//}

	//APawn* MyPawn = Cast<APawn>(GetOwner());

	//ISGameplayInterface::Execute_Interaction(FocusedActor, MyPawn);

	ServerInteract(FocusedActor);
	
}


void USInteractionComponent::ServerInteract_Implementation(AActor* InFcous)
{
	if (InFcous == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());

	ISGameplayInterface::Execute_Interaction(InFcous, MyPawn);
}


FVector USInteractionComponent::AttackLineDetection()
{
	bool bDebugDraw = CVarDebugDrawInteract.GetValueOnGameThread();

	//�ú������ڶ���Ļ���Ľ������߼�⣬ȷ����Ļ���ĵ�������������������ײ��������꣬�����з���
	//��MainCharacter��ʹ��Kismet�ĺ�����������λ��֮�����תֵ

	FCollisionObjectQueryParams ObjectQueryParams;
	//�����ײ��ѯ������
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	//ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	AActor* MyOwner = GetOwner();

	/*��ȡ��Ļ���ĵĶ�ά����ת��Ϊ��������ϵ������*/
	FVector CrosshairWorldLocation;
	FVector CrosshairWorldDirection;

	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(ViewportSize);    //��ȡ��ǰ��Ļ�Ĵ�С
		FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);    //��ȡ��Ļ����
		//DeprojectScreenToWorld���Ǵ�����Ļλ�õ����������ת��
		//Ȼ��ͨ�����ý�������ص�CrosshairWorldLocation��CrosshairWorldDirction���ֱ��Ӧλ�úͷ���
		bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
			UGameplayStatics::GetPlayerController(this, 0),
			CrosshairLocation,
			CrosshairWorldLocation,
			CrosshairWorldDirection);
	}

	//���ǽ�End����Ϊ��Ļ�����������겢���÷�������10000����λ
	FVector End = CrosshairWorldLocation + (CrosshairWorldDirection * 5000);


	// ʹ�������⣬����ʹ�����߼�⣬�������߼�������ؼ���ļ��,���ܻ���ּ�ⲻ�������
	FHitResult Hit;

	//�������߼������İ뾶
	float Radius = 20.f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, CrosshairWorldLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	AActor* HitActor = Hit.GetActor();
	if (bBlockingHit)
	{
		FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
		if (bDebugDraw)
		{
			DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
		}	
		End = Hit.ImpactPoint;
	}
	return End;
}






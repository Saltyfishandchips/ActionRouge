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


//控制台控制是否开启绘制Debug球体
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


	//Tick函数不管是在服务端还是客户端都会运行，因此我们通过本地检测来限制射线检测，只有当我们使用Controller时运行射线检测
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
	//添加碰撞查询的类型
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	//当控制器有输入时，GetActorEyesViewPoint可以获取Pawn的眼部位置和旋转，并赋值给传进去的实参
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	//我们将End设置为模型的眼部位置加上控制器看向的地方乘上500
	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	TArray<FHitResult> Hits;


	FCollisionShape Shape;
	Shape.SetSphere(TranceRadius);


	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	//清空FocusedActor
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
			//检查碰撞的物体是不是可交互的，需要注意使用前缀U的类，不要使用前缀I
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


	//GetActor()将返回被击中的Actor类
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

	//该函数用于对屏幕中心进行射线检测，确定屏幕中心点坐标射线中遇到的碰撞物体的坐标，并进行返回
	//在MainCharacter中使用Kismet的函数计算两个位置之间的旋转值

	FCollisionObjectQueryParams ObjectQueryParams;
	//添加碰撞查询的类型
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	//ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	AActor* MyOwner = GetOwner();

	/*获取屏幕中心的二维坐标转换为世界坐标系下坐标*/
	FVector CrosshairWorldLocation;
	FVector CrosshairWorldDirection;

	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(ViewportSize);    //获取当前屏幕的大小
		FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);    //获取屏幕中心
		//DeprojectScreenToWorld就是处理屏幕位置到世界坐标的转换
		//然后通过引用将结果返回到CrosshairWorldLocation和CrosshairWorldDirction，分别对应位置和方向。
		bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
			UGameplayStatics::GetPlayerController(this, 0),
			CrosshairLocation,
			CrosshairWorldLocation,
			CrosshairWorldDirection);
	}

	//我们将End设置为屏幕中心世界坐标并往该方向延伸10000个单位
	FVector End = CrosshairWorldLocation + (CrosshairWorldDirection * 5000);


	// 使用球体检测，而不使用射线检测，由于射线检测是像素级别的检测,可能会出现检测不出的情况
	FHitResult Hit;

	//设置射线检测球体的半径
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






// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"


void UWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//��������UE��GC���������գ����ƣ���UWorldUserWidget��һ�ؼ������ŵ�Actor��ʧ��
	//GC����Ҫһ��ʱ����ո�ָ�룬���������Ҫ�����жϸ�Actor�Ƿ�Ϊ��
	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();

		UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing Health widegt."));
		return;
	}

	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))
	{
		//��ȡ��Ļdpi��ʹ���˺���ʾ����ȷ��λ����
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= Scale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
	
}

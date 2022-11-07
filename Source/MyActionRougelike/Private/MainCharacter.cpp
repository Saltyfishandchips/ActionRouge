// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "AttributeComponent.h"
#include "GameFramework/PlayerController.h"
#include "ActionComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));

	CameraComp->SetupAttachment(SpringArmComp);
	SpringArmComp->SetupAttachment(RootComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true; //将运动组件下的 bOrientRotationToMovement 置为真使玩家转向加速度的反向 
	GetCharacterMovement()->bUseControllerDesiredRotation = true; //bUseControllerDesiredRotation 置为真使转向更自然。

	bUseControllerRotationYaw = false;

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComp"));

	//实例化角色属性
	AttributeComp = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComp"));

	//实例化角色行动GAS
	ActionComp = CreateDefaultSubobject<UActionComponent>("ActionComp");

}


void AMainCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &AMainCharacter::OnHealthChanged);
}


// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCharacter::MoveRight);


	//摄像机视角移动采用Pawn下的方法，直接获取Controller在Pitch和Yaw中的输入
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);

	//跳跃选项，首先是需要对bPressedJump这个bool值进行操作，确定键入的值的按下与释放
	//使用BindAction作为跳跃的功能
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMainCharacter::StartJump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AMainCharacter::StopJump);

	//冲刺函数
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AMainCharacter::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AMainCharacter::SprintStop);

	//宝箱交互函数
	PlayerInputComponent->BindAction(TEXT("PrimaryInteraction"), IE_Pressed, this, &AMainCharacter::PrimaryInteraction);

	//增加攻击函数
	PlayerInputComponent->BindAction(TEXT("PrimaryAttack"), IE_Pressed, this, &AMainCharacter::PrimaryAttack);

	//增加黑洞弹丸攻击函数
	PlayerInputComponent->BindAction(TEXT("BlackHoleAttack"), IE_Pressed, this, &AMainCharacter::BlackHoleAttack);

	//交互传送函数
	PlayerInputComponent->BindAction(TEXT("TeleportAttack"), IE_Pressed, this, &AMainCharacter::TeleportAttack);

}


void AMainCharacter::MoveForward(float value)
{
	//固定相机时使用，但是在摄像机视角移动时就会不自然
	//FVector Forward = GetActorForwardVector();
	//AddMovementInput(Forward, value);

	//需要首先获取摄像机的FRotator，只保留yaw的角度
	FRotator Forward = GetControlRotation();
	Forward.Pitch = 0.0f;
	Forward.Roll = 0.0f;

	//将Forward从FRotator转换为FVector
	AddMovementInput(Forward.Vector(), value);

}

void AMainCharacter::MoveRight(float value)
{
	//固定相机时使用，但是在摄像机视角移动时就会不自然
	//FVector Right = GetActorRightVector();
	//AddMovementInput(Right, value);

	//在角色右移操作时，我们同样的使用前进的操作
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	//通过FRotationMatrix拿到Controller的旋转矩阵，然后通过矩阵变换得到Y轴，也就是右移的FVector
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, value);
}

void AMainCharacter::StartJump()
{
	bPressedJump = true;
}

void AMainCharacter::StopJump()
{
	bPressedJump = false;
}

void AMainCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}


void AMainCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}


void AMainCharacter::PrimaryInteraction()
{
	//宝箱交互函数
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteraction();
	}

}


void AMainCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void AMainCharacter::BlackHoleAttack()
{
	ActionComp->StartActionByName(this, "BlackHoleAttack");
}

void AMainCharacter::TeleportAttack()
{
	ActionComp->StartActionByName(this, "TeleportAttack");
}


void AMainCharacter::OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta)
{

	//受到伤害
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);

		//Character受到伤害后增加怒气值
		float RageDelta = FMath::Abs(Delta);
		AttributeComp->ApplyRageChange(InstigatorActor, RageDelta);
	}
	
	//角色死亡
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		//由于GetController获得的类型是AController，因此我们需要使用Cast进行类型转换，此外，使用Alt+G可以快速查看函数的输出输入类型
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
		SetLifeSpan(5.f);
	}

}

//控制台治愈函数
void AMainCharacter::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}


FVector AMainCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

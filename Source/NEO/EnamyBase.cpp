// Fill out your copyright notice in the Description page of Project Settings.


#include "EnamyBase.h"
#include "GameSystem/TGS_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameSystem/Enemy_UMG.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"

// Sets default values
AEnamyBase::AEnamyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    bIsJumping = false;
    //重力加速度
    fGravityAcceleration = 9.8f;
    //ジャンプ時間
    fJumpTime = 0.0f;
    //ジャンプ高さ
    fJumpHeight = 100.0f;
    //ジャンプ開始時の位置
    vJumpStartLocation = GetActorLocation();

    //UMG Create
    EnemyWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Enemy_UMG"));
    EnemyWidget->SetupAttachment(RootComponent);

    //描画モード World
    EnemyWidget->SetWidgetSpace(EWidgetSpace::World);
}

void AEnamyBase::DestoryEnemy()
{
	ATGS_GameMode* GameMode =Cast<ATGS_GameMode>( UGameplayStatics::GetGameMode(GetWorld()) );
	if (GameMode)
	{
		GameMode->DestroyEnemy(this,IsAreaEnemy);
	}
}

// Called when the game starts or when spawned
void AEnamyBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();

    //UMG Set
    if (EnemyWidgetClass) {
	    EnemyWidget->SetWidgetClass(EnemyWidgetClass);

        //Cast EnemyuUMG
        UEnemy_UMG* EnemyUMG = Cast<UEnemy_UMG>(EnemyWidget->GetUserWidgetObject());
    if (EnemyUMG)		{
        FName Name = FName("Name");
        EnemyUMG->SetEnemyInfo(Health, MaxHealth,Name);
		}
    }

	
}

// Called every frame
void AEnamyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    SetFaceCamera();
}

// Called to bind functionality to input
void AEnamyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AEnamyBase::ApplyDamage(float DamageAmount, float DeltaTime)
{
	Health -= DamageAmount;

	if (Health <= 0.0f)
	{
		//ジャンプ処理
		JumpingByGravity(DeltaTime);
		PlayAnimMontage(Death, 1, NAME_None);

	}
	else
	{
		PlayAnimMontage(Damage_Reaction, 1, NAME_None);
	}
}
void AEnamyBase::SetFaceCamera()
{
    //カメラのを取得
	AActor* ViewTarget = GetWorld()->GetFirstPlayerController()->GetViewTarget();
	UCameraComponent* CameraComponent = ViewTarget->FindComponentByClass<UCameraComponent>();
	if (true) {

        FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();

        //カメラの位置を向く
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
            EnemyWidget->GetComponentLocation(), CameraLocation);

        //軸を固定
         LookAtRotation.Pitch = 0.0f;
         LookAtRotation.Roll = 0.0f;

         //回転設定
         EnemyWidget->SetWorldRotation(LookAtRotation);
	}
    else	{
		UE_LOG(LogTemp, Warning, TEXT("CameraComponent is null"));
	}
}
void AEnamyBase::StartJumpByGravity(float JumpHeight, float GravityAcceleration)
{
    if (!bIsJumping)
    {
        //ジャンプを開始
        fJumpTime = 0.0f;
        fJumpHeight = JumpHeight;
        fGravityAcceleration = GravityAcceleration;

        bIsJumping = true;
    }
}
void AEnamyBase::JumpingByGravity(float DeltaTime)
{
    if (bIsJumping)
    {
        //ジャンプ中
        fJumpTime += DeltaTime;

        //ジャンプの移動距離を計算
        float fCurrentJumpHeight = fJumpHeight * FMath::Sin(fJumpTime) - 0.5f * fGravityAcceleration * FMath::Pow(fJumpTime, 2.0f);

        //キャラクターに移動距離を反映
        FVector vCurrentLocation = GetActorLocation();
        vCurrentLocation.Z = vJumpStartLocation.Z + fCurrentJumpHeight;
        SetActorLocation(vCurrentLocation);

        //ジャンプを終了するかどうかを判断
        if (fCurrentJumpHeight <= 0.0f)
        {
            EndJumpByGravity();
        }

    }
}

/**
 * 重力ジャンプを終了する
 */
void AEnamyBase::EndJumpByGravity()
{
    //ジャンプを終了
    bIsJumping = false;

    //ジャンプ時間をリセット
    fJumpTime = 0.0f;

    //ジャンプ高さをリセット
    fJumpHeight = 0.0f;

    //重力加速度をリセット
    fGravityAcceleration = 0.0f;
}

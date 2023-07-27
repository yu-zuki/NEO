// Fill out your copyright notice in the Description page of Project Settings.


#include "EnamyBase.h"
#include "NEO/GameSystem/TGS_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/GameSystem/Enemy_UMG.h"
#include "Components/WidgetComponent.h"
#include "DeathTrigger.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "NEO/GameSystem/EnemyBase_WidgetComponent.h"

// Sets default values
AEnamyBase::AEnamyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//UI Create
	EnemyWidget = CreateDefaultSubobject<UEnemyBase_WidgetComponent>(TEXT("EnemyWidget"));
	EnemyWidget->SetupAttachment(RootComponent);
   
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


}

// Called every frame
void AEnamyBase::Tick(float DeltaTime)
{   
    if (bIsNowDamage)
    {
        return; 
    }
    Super::Tick(DeltaTime); 
    //Enemy Hp Set
	    EnemyWidget->SetHPInfo(Health, MaxHealth);
     
        //キャラクターの位置を取得
        FVector CharacterLocation = GetActorLocation();

        // 自分の座標を取得
        FVector MyLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	   
        // キャラクターの位置と自分の位置を比較してY軸より前にいるかどうかを判定
        bIsRotation = CharacterLocation.Y > MyLocation.Y;
        //bIsRotationがtrueなら
        if (Health >= 0)
        {
            if (bIsRotation)
            {
                FRotator NewRotation = GetActorRotation();
                NewRotation.Yaw = -90.0f;
                SetActorRotation(NewRotation);

            }
            else
            {
                FRotator NewRotation = GetActorRotation();
                NewRotation.Yaw = 90.0f;
                SetActorRotation(NewRotation);
            }

        }
}

// Called to bind functionality to input
void AEnamyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnamyBase::ApplyDamage(float DamageAmount)
{
    
	Health -= DamageAmount;
    FTimerHandle TimerHandle_ResetDamage;
    bIsNowDamage = true;
    GetWorldTimerManager().SetTimer(TimerHandle_ResetDamage, this, &AEnamyBase::DamageReac, 0.2f, false);
    if (Health <= 0)
    {
        SpawnDeathTrigger();
        PlayAnimMontage(Death, 0.8, NAME_None);
        GetWorldTimerManager().SetTimer(TimerHandle_DestroyEnemy, this, &AEnamyBase::AfterDeath, 1.6f, true);
    }
    else
    {
        PlayAnimMontage(Damage_Reaction, 0.8, NAME_None);
       
    }

}
void AEnamyBase::AfterDeath()
{
    DestoryEnemy();
}

void AEnamyBase::DamageReac()
{
    bIsNowDamage = false;
}

void AEnamyBase::SpawnDeathTrigger()
{
    UWorld* World = GetWorld();  // 現在のWorldを取得
    if (World)  // Worldが存在する場合のみスポーンを実行
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        // スポーンする位置と向きを取得（この場合、敵キャラクターと同じ位置と向き）
        FVector SpawnLocation = GetActorLocation();
        FRotator SpawnRotation = GetActorRotation();

        // DeathTriggerをスポーン
        ADeathTrigger* DeathTrigger = World->SpawnActor<ADeathTrigger>(ADeathTrigger::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams); 

        // DeathTriggerをEnemyBaseにアタッチ
        if (DeathTrigger)  // DeathTriggerが正常にスポーンされた場合のみアタッチを実行
        {
            DeathTrigger->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
}




// Fill out your copyright notice in the Description page of Project Settings.


#include "EnamyBase.h"
#include "EngineUtils.h"
#include "NEO/GameSystem/TGS_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/GameSystem/Enemy_UMG.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "NEO/GameSystem/EnemyBase_WidgetComponent.h"
#include "NEO/PlayerSystem/ActionAssistComponent.h"


// Sets default values
AEnamyBase::AEnamyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//UI Create
	EnemyWidget = CreateDefaultSubobject<UEnemyBase_WidgetComponent>(TEXT("EnemyWidget"));
	EnemyWidget->SetupAttachment(RootComponent);
    bIsDeath = false;
   
    // アタックアシストコンポーネント作成
    ActionAssistComp = CreateDefaultSubobject<UActionAssistComponent>(TEXT("AttackAssist"));
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

AActor* AEnamyBase::GetPlayer()
{
    APawn* PlayerChara = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    return PlayerChara;
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
        UWorld* World = GetWorld();
        if (World)
        {
            APlayerController* PlayerController = World->GetFirstPlayerController();
            if (PlayerController)
            {
                APawn* Pawn = PlayerController->GetPawn();
                if (Pawn)
                {
                    FVector MyLocation = Pawn->GetActorLocation();
                    // キャラクターの位置と自分の位置を比較してY軸より前にいるかどうかを判定
                    bIsRotation = CharacterLocation.Y > MyLocation.Y;
                    //bIsRotationがtrueなら
                    if (Health >= 0)
                    {
                        //if (bIsRotation)
                        //{
                        //    FRotator NewRotation = GetActorRotation();
                        //    NewRotation.Yaw = -90.0f;
                        //    SetActorRotation(NewRotation);

                        //    

                        //}
                        //else
                        //{
                        //    FRotator NewRotation = GetActorRotation();
                        //    NewRotation.Yaw = 90.0f;
                        //    SetActorRotation(NewRotation);
                        //}

                        bool LookRight = (bIsRotation) ? (true) : (false);


                        ActionAssistComp->OwnerParallelToCamera(LookRight);

                    }
                }
            }
        }
       
        CheckHealth();
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
        
        PlayAnimMontage(Death, 0.8, NAME_None);
        GetWorldTimerManager().SetTimer(TimerHandle_DestroyEnemy, this, &AEnamyBase::AfterDeath, 1.6f, true);
        bIsDeath = true;
        FVector ForceDirection = -GetActorForwardVector();
        float ForceStrength = 1000.0f;  // Adjust this value as needed.
        GetCharacterMovement()->AddForce(ForceDirection * ForceStrength);
        GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
        GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

    }
    else
    {
        PlayAnimMontage(Damage_Reaction, 0.8, NAME_None);
       
        ActionAssistComp->SpawnHitEffect(NiagaraEffect, GetActorLocation());
       
    }

}
void AEnamyBase::MaintainDistanceFromEnemy()
{
    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        AActor* CurrentActor = *It;
        if (CurrentActor && CurrentActor->ActorHasTag(FName("Enemy")))
        {
            FVector Direction = CurrentActor->GetActorLocation() - GetActorLocation();
            float Distance = Direction.Size();

            if (Distance < DesiredDistanceFromEnemy)
            {
                FVector MoveDirection = -Direction.GetSafeNormal() * (DesiredDistanceFromEnemy - Distance);
                AddMovementInput(MoveDirection);
            }
        }
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

void AEnamyBase::CheckHealth()
{
    if (Health <= 0)
    {
        SpawnDeathTrigger();
    }
}

void AEnamyBase::SpawnDeathTrigger()
{
    if (DeathTriggerClass = nullptr)
    {
        FTransform SpawnTransform = GetActorTransform();
        ADeathTrigger* DeathTrigger = GetWorld()->SpawnActor<ADeathTrigger>(DeathTriggerClass, SpawnTransform);
        if (DeathTriggerClass != nullptr)
        {
            DeathTrigger->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
}

AActor* AEnamyBase::GetEnemyActor() const
{
    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;
        if (Actor && Actor->ActorHasTag(FName("Enemy")))
        {
            return Actor; // Enemyタグを持つ最初のアクターを返す
        }
    }
    return nullptr; // Enemyタグを持つアクターが見つからない場合
}





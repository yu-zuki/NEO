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
     
        //�L�����N�^�[�̈ʒu���擾
        FVector CharacterLocation = GetActorLocation();

        // �����̍��W���擾
        FVector MyLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	   
        // �L�����N�^�[�̈ʒu�Ǝ����̈ʒu���r����Y�����O�ɂ��邩�ǂ����𔻒�
        bIsRotation = CharacterLocation.Y > MyLocation.Y;
        //bIsRotation��true�Ȃ�
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
    UWorld* World = GetWorld();  // ���݂�World���擾
    if (World)  // World�����݂���ꍇ�̂݃X�|�[�������s
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        // �X�|�[������ʒu�ƌ������擾�i���̏ꍇ�A�G�L�����N�^�[�Ɠ����ʒu�ƌ����j
        FVector SpawnLocation = GetActorLocation();
        FRotator SpawnRotation = GetActorRotation();

        // DeathTrigger���X�|�[��
        ADeathTrigger* DeathTrigger = World->SpawnActor<ADeathTrigger>(ADeathTrigger::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams); 

        // DeathTrigger��EnemyBase�ɃA�^�b�`
        if (DeathTrigger)  // DeathTrigger������ɃX�|�[�����ꂽ�ꍇ�̂݃A�^�b�`�����s
        {
            DeathTrigger->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
}




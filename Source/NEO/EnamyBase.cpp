// Fill out your copyright notice in the Description page of Project Settings.


#include "EnamyBase.h"
#include "GameSystem/TGS_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameSystem/Enemy_UMG.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameSystem/EnemyBase_WidgetComponent.h"

// Sets default values
AEnamyBase::AEnamyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    bIsJumping = false;
    //�d�͉����x
    fGravityAcceleration = 9.8f;
    //�W�����v����
    fJumpTime = 0.0f;
    //�W�����v����
    fJumpHeight = 100.0f;
    //�W�����v�J�n���̈ʒu
    vJumpStartLocation = GetActorLocation();

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
	Super::Tick(DeltaTime);

	//Enemy Hp Set
	EnemyWidget->SetHPInfo(Health, MaxHealth);
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
		//�W�����v����
		JumpingByGravity(DeltaTime);
		PlayAnimMontage(Death, 1, NAME_None);

	}
	else
	{
		PlayAnimMontage(Damage_Reaction, 1, NAME_None);
	}
}
void AEnamyBase::StartJumpByGravity(float JumpHeight, float GravityAcceleration)
{
    if (!bIsJumping)
    {
        //�W�����v���J�n
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
        //�W�����v��
        fJumpTime += DeltaTime;

        //�W�����v�̈ړ��������v�Z
        float fCurrentJumpHeight = fJumpHeight * FMath::Sin(fJumpTime) - 0.5f * fGravityAcceleration * FMath::Pow(fJumpTime, 2.0f);

        //�L�����N�^�[�Ɉړ������𔽉f
        FVector vCurrentLocation = GetActorLocation();
        vCurrentLocation.Z = vJumpStartLocation.Z + fCurrentJumpHeight;
        SetActorLocation(vCurrentLocation);

        //�W�����v���I�����邩�ǂ����𔻒f
        if (fCurrentJumpHeight <= 0.0f)
        {
            EndJumpByGravity();
        }

    }
}

/**
 * �d�̓W�����v���I������
 */
void AEnamyBase::EndJumpByGravity()
{
    //�W�����v���I��
    bIsJumping = false;

    //�W�����v���Ԃ����Z�b�g
    fJumpTime = 0.0f;

    //�W�����v���������Z�b�g
    fJumpHeight = 0.0f;

    //�d�͉����x�����Z�b�g
    fGravityAcceleration = 0.0f;
}

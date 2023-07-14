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
    //�d�͉����x
    fGravityAcceleration = 9.8f;
    //�W�����v����
    fJumpTime = 0.0f;
    //�W�����v����
    fJumpHeight = 100.0f;
    //�W�����v�J�n���̈ʒu
    vJumpStartLocation = GetActorLocation();

    //UMG Create
    EnemyWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Enemy_UMG"));
    EnemyWidget->SetupAttachment(RootComponent);

    //�`�惂�[�h World
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
		//�W�����v����
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
    //�J�����̂��擾
	AActor* ViewTarget = GetWorld()->GetFirstPlayerController()->GetViewTarget();
	UCameraComponent* CameraComponent = ViewTarget->FindComponentByClass<UCameraComponent>();
	if (true) {

        FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();

        //�J�����̈ʒu������
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
            EnemyWidget->GetComponentLocation(), CameraLocation);

        //�����Œ�
         LookAtRotation.Pitch = 0.0f;
         LookAtRotation.Roll = 0.0f;

         //��]�ݒ�
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "GunMan.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/World.h"
#include "Bullet.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGunMan::AGunMan()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    // �L�����N�^�[�̈ړ�������ݒ�
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
    MaxHealth = 100;
    Health = MaxHealth;
    bIsJumping = false;
    //�d�͉����x
    fGravityAcceleration = 9.8f;
    //�W�����v����
    fJumpTime = 0.0f;
    //�W�����v����
    fJumpHeight = 100.0f;
    //�W�����v�J�n���̈ʒu
    vJumpStartLocation = GetActorLocation();
}

// Called when the game starts or when spawned
void AGunMan::BeginPlay()
{
	Super::BeginPlay();
    // �v���C���[�L�����N�^�[�̎Q�Ƃ��擾
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AGunMan::SpawnBullet, 3.0f, true);
}
    


// Called every frame
void AGunMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Health = MaxHealth;
    FacePlayer();
    if (PlayerCharacter)
    {
        // �v���C���[�Ƃ̋������擾
        float DistanceToPlayer = GetDistanceToPlayer();

        // �v���C���[�Ƃ̋������]�܂���������������Ă���ꍇ�A�v���C���[�ɋ߂Â�
        if (DistanceToPlayer > DesiredDistance)
        {
            FVector PlayerDirection = GetPlayerDirection();
            AddMovementInput(PlayerDirection);
            //�v���C���[�̂ق�������
            FRotator TargetRotation = PlayerDirection.Rotation();
            SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.0f));
        }
        else if (DistanceToPlayer < DesiredDistance - 100.0f) // �v���C���[���]�܂�������-100�ȉ��ɓ������ꍇ
        {
            // �v���C���[���痣���
            FVector PlayerDirection = GetPlayerDirection();
            AddMovementInput(-PlayerDirection);
        }
    }
}
void AGunMan::SpawnBullet()
{
    if (BulletClass)
    {
        FActorSpawnParameters SpawnParams;
        GetWorld()->SpawnActor<ABullet>(BulletClass, GetActorLocation() + FVector(0.f, 0.f, 50.f), GetActorRotation(), SpawnParams);
    }
}
FVector AGunMan::GetPlayerDirection() const
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector GunManLocation = GetActorLocation();
    return FVector(PlayerLocation.X - GunManLocation.X, PlayerLocation.Y - GunManLocation.Y, 0.0f).GetSafeNormal();
}

float AGunMan::GetDistanceToPlayer() const
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector GunManLocation = GetActorLocation();
    return FVector::Distance(PlayerLocation, GunManLocation);
}

void AGunMan::FacePlayer()
{
    // �v���C���[�L�����N�^�[�̈ʒu���擾
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();

    // �v���C���[�L�����N�^�[�̈ʒu�Ɍ�������Lancer����]������
    FRotator TargetRotation = (PlayerLocation - GetActorLocation()).Rotation();
    SetActorRotation(TargetRotation);
}
void AGunMan::ApplyDamage(float DamageAmount, float DeltaTime)
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
void AGunMan::StartJumpByGravity(float JumpHeight, float GravityAcceleration)
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
void AGunMan::JumpingByGravity(float DeltaTime)
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
void AGunMan::EndJumpByGravity()
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

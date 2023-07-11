#include "Soldier.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "AIController.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

ASoldier::ASoldier()
{
    // �L�����N�^�[�̈ړ��������
    MaxHealth = 100;
    Health = MaxHealth;
    bIsJumping = false;
    // �L�����N�^�[�̈ړ�������ݒ�
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    
    //�d�͉����x
    fGravityAcceleration = 9.8f;
    //�W�����v����
    fJumpTime = 0.0f;
    //�W�����v����
    fJumpHeight = 100.0f;
    //�W�����v�J�n���̈ʒu
    vJumpStartLocation = GetActorLocation();
    //�U���̊Ԋu
    DelayBeforeAttack = 2.0f;
    AttackInterval = 5.0f;
    MoveSpeed = 600.0f;
}



void ASoldier::BeginPlay()
{
    Super::BeginPlay();

    // �v���C���[�L�����N�^�[�̎Q�Ƃ��擾
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    // 2�b��ɍU�����J�n����
    GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ASoldier::StartAttack, DelayBeforeAttack, false);
}


void ASoldier::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // �L�����N�^�[�̈ʒu���擾
    FVector CharacterLocation = GetActorLocation();

    // �����̍��W���擾
    FVector MyLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

    // �L�����N�^�[�̈ʒu�Ǝ����̈ʒu���r����Y�����O�ɂ��邩�ǂ����𔻒�
    bIsRotation = CharacterLocation.Y > MyLocation.Y;
    // bIsRotation��true�Ȃ�
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
void ASoldier::StartAttack()
{
    // �v���C���[�Ɍ������ēˌ�����
    MoveToPlayer();

    // �w��̊Ԋu�ōU�����J��Ԃ�
    GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ASoldier::RepeatAttack, AttackInterval, true);
}
void ASoldier::RepeatAttack()
{
    /*
     // �v���C���[�Ƃ̋������v�Z
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (PlayerPawn)
    {
        FVector PlayerLocation = PlayerPawn->GetActorLocation();
        FVector SoldierLocation = GetActorLocation();
        float Distance = FVector::Dist(PlayerLocation, SoldierLocation);

        // �v���C���[�Ƃ̋��������͈͓��łȂ���΃v���C���[�Ɍ������ēˌ�����
        if (Distance > MaxDistanceFromPlayer)
        {
            MoveToPlayer();
        }
    
     }
    */
   
    MoveToPlayer();
}

void ASoldier::MoveToPlayer()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (PlayerPawn)
    {
        PlayerLocation = PlayerPawn->GetActorLocation();

        // �v���C���[�̕����ɐi��
        FVector Direction = PlayerLocation - GetActorLocation();
        Direction.Z = 0.0f;
        Direction.Normalize();

        // �ړ����x��ݒ�
        UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
        if (MovementComponent)
        {
            MovementComponent->MaxWalkSpeed = MoveSpeed;
        }

        // �܂������i��
        AddMovementInput(Direction, 1.0f);
    }
}
void ASoldier::ApplyDamage(float DamageAmount, float DeltaTime)
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
void ASoldier::StartJumpByGravity(float JumpHeight, float GravityAcceleration)
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
void ASoldier::JumpingByGravity(float DeltaTime)
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
void ASoldier::EndJumpByGravity()
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
// Called to bind functionality to input
void ASoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}






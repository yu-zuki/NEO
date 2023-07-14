#include "Soldier.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
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

    
    PrimaryActorTick.bCanEverTick = true;
    MovementRadius = 500.0f; // �ړ��͈͂̔��a��ݒ�
    bIsLocation = true;
}



void ASoldier::BeginPlay()
{
    Super::BeginPlay();

    InitialLocation = GetActorLocation(); // �����ʒu��ۑ�

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
    // �����_���Ȉړ��x�N�g���𐶐�
    FVector RandomMovement = FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), 0.0f);
    RandomMovement.Normalize();

    // �ړ��͈͓��̃����_���Ȉʒu���v�Z
    FVector TargetLocation = InitialLocation + RandomMovement * MovementRadius;

    // �v���C���[�Ƃ̋��������͈͓��ł���Έړ�����
    FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
    float DistanceToPlayer = FVector::Dist(InitialLocation, PlayerLocation);
    bIsRotation = CharacterLocation.Y > MyLocation.Y;
   
     if (DistanceToPlayer > MovementRadius)
        {
        // �ړ���Ɍ������Ĉړ�����
        FVector MovementDirection = TargetLocation - GetActorLocation();
        MovementDirection.Normalize();
        SetActorLocation(GetActorLocation() + MovementDirection * DeltaTime * GetCharacterMovement()->MaxWalkSpeed);
        
       
        }
    
   
    else
    {
        FVector ForwardDirection = GetActorForwardVector();
        SetActorLocation(GetActorLocation() + ForwardDirection *10.0f);
        
    }

}
void ASoldier::EnableTick()
{
    SetActorTickEnabled(true);
}
// Called to bind functionality to input
void ASoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}







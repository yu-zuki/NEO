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
    // �L�����N�^�[�̈ړ�������ݒ�
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
    Health = MaxHealth;
  
}



void ASoldier::BeginPlay()
{
    Super::BeginPlay();

    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

}


void ASoldier::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // �v���C���[�Ƃ̋������擾
    float DistanceToPlayer = GetDistanceToPlayer();

    // �v���C���[�Ƃ̋������]�܂���������������Ă���ꍇ�A�v���C���[�ɋ߂Â�
    if (DistanceToPlayer > DesiredDistance)
    {
        FVector PlayerDirection = GetPlayerDirection();
        AddMovementInput(PlayerDirection);
        MovementSpeed = 100.0f;
        bIsRotation = true;
    }
    else if (DistanceToPlayer < DesiredDistance +100.0f) // �v���C���[���]�܂�������-100�ȉ��ɓ������ꍇ
    {
        
        MovementSpeed = 200.0f;
        
        bIsRotation = false;
    }
  

}

FVector ASoldier::GetPlayerDirection() const
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector GunManLocation = GetActorLocation();
    return FVector(PlayerLocation.X - GunManLocation.X, PlayerLocation.Y - GunManLocation.Y, 0.0f).GetSafeNormal();
}

float ASoldier::GetDistanceToPlayer() const
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector GunManLocation = GetActorLocation();
    return FVector::Distance(PlayerLocation, GunManLocation);
}







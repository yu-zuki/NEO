// Fill out your copyright notice in the Description page of Project Settings.


#include "GunMan.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NEO/PlayerSystem\PlayerCharacter.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Bullet.h"
#include "TrajectoryBullet.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NEO/BackGroundSystem/GunFence.h"
#include "NavigationPath.h"
#include "Kismet/KismetMathLibrary.h"
#include "NEO/WeaponSystem/WeaponBase.h"


// Sets default values
AGunMan::AGunMan()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;


    MaxHealth = 60;
    Health = MaxHealth;
    bIsBulletAlive = false;
    bCanMove = true;
}


/*
 * �֐����@�@�@�@�FAGunMan::BeginPlay
 * �������e�@�@�@�F�����ݒ���s���܂��B�^�C�}�[�̐ݒ�A����̃X�|�[���ȂǁB
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */void AGunMan::BeginPlay()
{
    Super::BeginPlay();
    // �v���C���[�L�����N�^�[�̎Q�Ƃ��擾
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

    GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AGunMan::PlayAttackAnim, 5.0f, true);

    // �����Spawn
    if (WeaponClass && !Weapon)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, SpawnParams);

        if (Weapon)
        {
            Weapon->AttachToHand(this, "enemy_R_handGun", EOwnerType::OwnerType_Enemy);
        }
    }
}
 /*
 * �֐����@�@�@�@�FAGunMan::GetSnappedDirection
 * �������e�@�@�@�F�^����ꂽ�����x�N�g���𒲐����܂��B
 * �߂�l�@�@�@�@�F�������ꂽ�����x�N�g���iFVector�j
 */
FVector AGunMan::GetSnappedDirection(const FVector& Direction) const
{
    FVector SnappedDirection = Direction;

    if (FMath::Abs(SnappedDirection.X) > FMath::Abs(SnappedDirection.Y))
    {
        SnappedDirection.Y = 0.0f;
    }
    else
    {
        SnappedDirection.X = 0.0f;
    }

    return SnappedDirection.GetSafeNormal();
}
/*
 * �֐����@�@�@�@�FAGunMan::CollisionOn
 * �������e�@�@�@�F����̃R���W������L���ɂ��܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */

void AGunMan::CollisionOn()
{
    if (Weapon)
    {
        Weapon->SetCollision();
    }
}
/*
 * �֐����@�@�@�@�FAGunMan::Tick
 * �������e�@�@�@�F���t���[���Ăяo����A�L�����N�^�[�̏�Ԃ�ʒu���X�V���܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void AGunMan::Tick(float DeltaTime)
{

    Super::Tick(DeltaTime);
    if (Health <= 0)
    {
        if (Weapon)
        {
            Weapon->DetachToHand();
            Weapon = nullptr;
        }
    }
  

    if (bIsNowDamage)
    {
        return;
    }
    PlayerCharacter = Cast<ACharacter>(GetPlayer());
    if (!PlayerCharacter) return;

    float CurrentDistance = GetDistanceToPlayer();
    FVector DirectionToPlayer = GetPlayerDirection();
    FVector SnappedDirection;
    FVector MoveVector;

    if (bCanMove == true)
    {
        if (CurrentDistance > DesiredDistance)
        {
            SnappedDirection = GetSnappedDirection(DirectionToPlayer);
            MoveVector = SnappedDirection * MoveSpeed * DeltaTime;
            IsRunning = true;
            IsIdol = false;
        }
        else if (CurrentDistance < DesiredDistance + 400)
        {
            SnappedDirection = GetSnappedDirection(-DirectionToPlayer);
            MoveVector = SnappedDirection * MoveSpeed / 3 * DeltaTime;
            IsRunning = true;
            IsIdol = false;
        }
        else
        {
            return; // ���̑��̏ꍇ�͈ړ����Ȃ�
        }

        SetActorLocation(GetActorLocation() + MoveVector);
    }
    FVector Start = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();
    FVector End = ((ForwardVector * 15.f) + Start);  // 200 �̓g���[�X�̋����A�K�X�������邱��

    FHitResult HitResult;

    // �g���[�X�̐ݒ�
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);  // �������g�͖�������

    // Line Trace ���s
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

    // �q�b�g�����ꍇ
    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor && HitActor->Tags.Contains("Fence"))
        {
            // Fence �^�O�����I�u�W�F�N�g���ڂ̑O�ɂ���I
            // ��������������
            IsRunning = false;
            IsIdol = true;
            bCanMove = false;
            MoveSpeed = 0;
        }
       
    }
    else
    {
        IsRunning = true;
        IsIdol = false;
        bCanMove = true;
        MoveSpeed = 100;
    }
}

/*
 * �֐����@�@�@�@�FAGunMan::GetPlayerDirection
 * �������e�@�@�@�F�v���C���[�ւ̕����x�N�g�����擾���܂��B
 * �߂�l�@�@�@�@�F�v���C���[�ւ̕����x�N�g���iFVector�j
 */

FVector AGunMan::GetPlayerDirection() const
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector GunManLocation = GetActorLocation();
    return FVector(PlayerLocation.X - GunManLocation.X, PlayerLocation.Y - GunManLocation.Y, 0.0f).GetSafeNormal();
}
/*
 * �֐����@�@�@�@�FAGunMan::GetDistanceToPlayer
 * �������e�@�@�@�F�v���C���[�܂ł̋������擾���܂��B
 * �߂�l�@�@�@�@�F�v���C���[�܂ł̋����ifloat�j
 */
float AGunMan::GetDistanceToPlayer() const
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector GunManLocation = GetActorLocation();
    return FVector::Distance(PlayerLocation, GunManLocation);
}
/*
 * �֐����@�@�@�@�FAGunMan::SpawnTrajectoryBullet
 * �������e�@�@�@�F�g���W�F�N�g���[�o���b�g���X�|�[�����܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void AGunMan::SpawnTrajectoryBullet()
{


    USkeletalMeshComponent* CharacterMesh = GetMesh();
    FVector SpawnLocation = CharacterMesh->GetSocketLocation("enemy_root");
    FRotator SpawnRotation = CharacterMesh->GetSocketRotation("enemy_root");
    if (TrajectoryBulletClass)
    {
        AActor* SpawnedTrajectoryBullet = GetWorld()->SpawnActor<AActor>(TrajectoryBulletClass, SpawnLocation, SpawnRotation);
        if (SpawnedTrajectoryBullet)
        {
            SpawnedTrajectoryBullet->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "enemy_root");
        }
    }
}
/*
 * �֐����@�@�@�@�FAGunMan::ReplaceWithBullet
 * �������e�@�@�@�F�o���b�g�ƒu�������܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void AGunMan::ReplaceWithBullet()
{
   
    FVector SpawnBulletLocation = GetActorLocation(); // �܂��͑��̏ꏊ
    FRotator SpawnBulletRotation = GetActorRotation(); // �܂��͑��̉�]
    if (BulletClass)
    {
        AActor* SpawnedBullet = GetWorld()->SpawnActor<AActor>(BulletClass, SpawnBulletLocation, SpawnBulletRotation);


    }
}
/*
 * �֐����@�@�@�@�FAGunMan::ResumeMovement
 * �������e�@�@�@�F�ړ����ĊJ���܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void AGunMan::ResumeMovement()
{
    // Resume movement
    GetCharacterMovement()->Activate(true);

}
/*
 * �֐����@�@�@�@�FAGunMan::UnlockRotation
 * �������e�@�@�@�F��]���A�����b�N���܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void AGunMan::UnlockRotation()
{
    bIsSpawningBullet = false;
}
/*
 * �֐����@�@�@�@�FAGunMan::PlayAttackAnim
 * �������e�@�@�@�F�U���A�j���[�V�������Đ����܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void AGunMan::PlayAttackAnim()
{
    if (Health > 0)
    {
        PlayAnimMontage(Attack, 1.0f, NAME_None);
        GetWorldTimerManager().SetTimer(TickEnableTimerHandle, this, &AGunMan::EnableTickAfterDelay, 3.0f, false);
        MoveSpeed = 0;
        IsRunning = false;
        IsIdol = true;
    }

}
/*
*�֐����@�@�@�@�FAGunMan::EnableTickAfterDelay
* �������e�@�@�@�F�f�B���C���Tick��L�������܂��B
* �߂�l�@�@�@�@�F�Ȃ��ivoid�j
*/
void AGunMan::EnableTickAfterDelay()
{
    
    MoveSpeed = 100;
}
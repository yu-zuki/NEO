#include "Lancer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/WeaponSystem/WeaponBase.h"


ALancer::ALancer()
{
    // �L�����N�^�[�̈ړ��������
    MaxHealth = 100;
    Health = MaxHealth;
    bIsRandMove = false;
    // �L�����N�^�[�̈ړ�������ݒ�
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
    IsRunning = false;
    IsIdol = true;
}


/*
 * �֐����@�@�@�@�FALancer::BeginPlay
 * �������e�@�@�@�F�����ݒ���s���܂��B�^�C�}�[�̐ݒ�A����̃X�|�[���ȂǁB
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void ALancer::BeginPlay()
{
    Super::BeginPlay();

    // �v���C���[�L�����N�^�[�̎Q�Ƃ��擾
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    // 5�b���Ƃ�CheckPlayerInFront�֐������s����^�C�}�[���Z�b�g
    GetWorldTimerManager().SetTimer(TimerHandle_CheckPlayerInFront, this, &ALancer::CheckPlayerInFront, 3.0f, true);
    GetWorld()->GetTimerManager().SetTimer(MoveToTargetTimer, this, &ALancer::ChooseNewTarget, 3.0f, true);
    SpawnTime = GetWorld()->GetTimeSeconds();
    // �����Spawn
    if (WeaponClass && !Weapon)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, SpawnParams);

        if (Weapon)
        {
            Weapon->AttachToHand(this, "enemy_R_handLance", EOwnerType::OwnerType_Enemy);
        }
    }
}
/*
 * �֐����@�@�@�@�FALancer::GetSnappedDirection
 * �������e�@�@�@�F�^����ꂽ�����x�N�g���𒲐����܂��B
 * �߂�l�@�@�@�@�F�������ꂽ�����x�N�g���iFVector�j
 */

FVector ALancer::GetSnappedDirection(const FVector& Direction) const
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
 * �֐����@�@�@�@�FALancer::CollisionOn
 * �������e�@�@�@�F����̃R���W������L���ɂ��܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void ALancer::CollisionOn()
{
    if (Weapon)
    {
        Weapon->SetCollision();
    }
}
/*
 * �֐����@�@�@�@�FALancer::Tick
 * �������e�@�@�@�F���t���[���Ăяo����A�L�����N�^�[�̏�Ԃ�ʒu���X�V���܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void ALancer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsNowDamage || bShouldSkipNextMovement)
    {
        bShouldSkipNextMovement = false;
        return;
    }
    
   
    
   
    PlayerCharacter = Cast<ACharacter>(GetPlayer());
    if (!PlayerCharacter) return;

    float CurrentDistance = GetDistanceToPlayer();
    FVector DirectionToPlayer = GetPlayerDirection();
    FVector SnappedDirection;
    FVector MoveVector;
    

    if (CurrentDistance < DesiredDistance+100 )
    {
        bIsRandMove = true;
    }
    else if(CurrentDistance > DesiredDistance - 100)
    {
        bIsRandMove = false;
    }
    if (Health > 0)
    {

        
        if (CurrentDistance > DesiredDistance&& bIsRandMove==false)
        {
            SnappedDirection = GetSnappedDirection(DirectionToPlayer);
            MoveVector = SnappedDirection * Speed*3 * DeltaTime;
            IsRunning = true;
            IsIdol = false;
           
        }
       else if(CurrentDistance < DesiredDistance + 100 && CurrentTarget&& bIsRandMove==true) // DesiredDistance���400m�����ꍇ
       {
           FVector DirectionToTarget = (CurrentTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
           MoveVector = DirectionToTarget * Speed/2* DeltaTime;
           IsRunning = true;
           IsIdol = false;
           
       }

      
       else if (FMath::Abs(CurrentDistance - DesiredDistance) < 10.0f)
        {
            SnappedDirection = GetSnappedDirection(DirectionToPlayer);
            MoveVector = SnappedDirection * Speed * 3 * DeltaTime;
            IsRunning = false;
            IsIdol = true;
        }
    else
    {
            IsRunning = false;
            IsIdol = true;
    }
    }
    if (Health <= 0)
    {
        if (Weapon)
        {
            Weapon->DetachToHand();
            Weapon = nullptr;
        }
    }
 
    SetActorLocation(GetActorLocation() + MoveVector);
    
}
/*
 * �֐����@�@�@�@�FALancer::ChooseNewTarget
 * �������e�@�@�@�F�V�����^�[�Q�b�g�������_���ɑI�����܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void ALancer::ChooseNewTarget()
{
    TArray<AActor*> FoundTargetPoints;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Random"), FoundTargetPoints);

    if (FoundTargetPoints.Num() > 0)
    {
        // �����_����TargetPoint��I������
        CurrentTarget = Cast<ATargetPoint>(FoundTargetPoints[FMath::RandRange(0, FoundTargetPoints.Num() - 1)]);
    }
}
/*
 * �֐����@�@�@�@�FALancer::GetPlayerDirection
 * �������e�@�@�@�F�v���C���[�ւ̕����x�N�g�����擾���܂��B
 * �߂�l�@�@�@�@�F�v���C���[�ւ̕����x�N�g���iFVector�j
 */
FVector ALancer::GetPlayerDirection() const
{
    if (!PlayerCharacter) return FVector::ZeroVector;

    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector LancerLocation = GetActorLocation();
    return FVector(PlayerLocation.X - LancerLocation.X, PlayerLocation.Y - LancerLocation.Y, 0.0f).GetSafeNormal();
}
/*
 * �֐����@�@�@�@�FALancer::GetDistanceToPlayer
 * �������e�@�@�@�F�v���C���[�܂ł̋������擾���܂��B
 * �߂�l�@�@�@�@�F�v���C���[�܂ł̋����ifloat�j
 */
float ALancer::GetDistanceToPlayer() const
{
    if (!PlayerCharacter) return 0.0f;

    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector LancerLocation = GetActorLocation();
    return FVector::Distance(PlayerLocation, LancerLocation);
}
/*
 * �֐����@�@�@�@�FALancer::CheckPlayerInFront
 * �������e�@�@�@�F�v���C���[��Lancer�̑O�ɂ��邩�ǂ������m�F���܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void ALancer::CheckPlayerInFront()
{
    // �����̈ʒu���擾
    FVector MyLocation = GetActorLocation();
    IsRunning = true;
    IsIdol = false;
    if (bIsNowDamage || bShouldSkipNextMovement)
    {
        bShouldSkipNextMovement = false;
        return;
    }
        UWorld* World = GetWorld();
        if (World)
        {
            APlayerController* PlayerController = World->GetFirstPlayerController();
            if (PlayerController)
            {
                APawn* Pawn = PlayerController->GetPawn();
                if (Pawn)
                {
                    FVector PlayerLocation = Pawn->GetActorLocation();
                    // ���v���C���[��Lancer�̖ڂ̑O�ɂ��邩�ǂ����𔻒�
                    FVector DirectionToPlayer = PlayerLocation - MyLocation;
                    float DotProduct = FVector::DotProduct(DirectionToPlayer.GetSafeNormal(), GetActorForwardVector());
                    bIsPlayerInFront = DotProduct > 0.0f;

                    if (bIsPlayerInFront&& Health > 0)
                    {
                        if (FMath::FRand() < 0.8f)
                        {
                            PlayAnimMontage(Attack, 1, NAME_None);
                              
                        }

                    }
                }
            }
        }
    
   
    
}













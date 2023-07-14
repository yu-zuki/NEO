#include "Lancer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

ALancer::ALancer()
{
    // �L�����N�^�[�̈ړ��������
    MaxHealth = 100;
    Health = MaxHealth;
    bIsJumping = false;
    // �L�����N�^�[�̈ړ�������ݒ�
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
   

}



void ALancer::BeginPlay()
{
    Super::BeginPlay();

    // �v���C���[�L�����N�^�[�̎Q�Ƃ��擾
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    // 5�b���Ƃ�CheckPlayerInFront�֐������s����^�C�}�[���Z�b�g
    GetWorldTimerManager().SetTimer(TimerHandle_CheckPlayerInFront, this, &ALancer::CheckPlayerInFront, 3.0f, true);
}


void ALancer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (PlayerCharacter)
    {
        // �v���C���[�Ƃ̋������擾
        float DistanceToPlayer = GetDistanceToPlayer();
        // �v���C���[�Ƃ̋������]�܂���������������Ă���ꍇ�A�v���C���[�ɋ߂Â�
        if (DistanceToPlayer > DesiredDistance)
        {
            FVector PlayerDirection = GetPlayerDirection();
            AddMovementInput(PlayerDirection);
        }
    }
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
void ALancer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
FVector ALancer::GetPlayerDirection() const
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector LancerLocation = GetActorLocation();
    return FVector(PlayerLocation.X - LancerLocation.X, PlayerLocation.Y - LancerLocation.Y, 0.0f).GetSafeNormal();
}
float ALancer::GetDistanceToPlayer() const
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector LancerLocation = GetActorLocation();
    return FVector::Distance(PlayerLocation, LancerLocation);
}

void ALancer::CheckPlayerInFront()
{
    // �����̈ʒu���擾
    FVector MyLocation = GetActorLocation();

    // ���v���C���[�̈ʒu���擾
    FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

    // ���v���C���[��Lancer�̖ڂ̑O�ɂ��邩�ǂ����𔻒�
    FVector DirectionToPlayer = PlayerLocation - MyLocation;
    float DotProduct = FVector::DotProduct(DirectionToPlayer.GetSafeNormal(), GetActorForwardVector());
    bIsPlayerInFront = DotProduct > 0.0f;

    if (bIsPlayerInFront)
    {
        PlayAnimMontage(Attack, 1, NAME_None);
    }
}




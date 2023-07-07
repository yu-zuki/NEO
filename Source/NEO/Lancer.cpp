#include "Lancer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

ALancer::ALancer()
{
    // �L�����N�^�[�̈ړ��������
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



void ALancer::BeginPlay()
{
    Super::BeginPlay();

    // �v���C���[�L�����N�^�[�̎Q�Ƃ��擾
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
}

void ALancer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // Make the character's Y-axis always face towards the camera/player
    if (GEngine && GEngine->GetFirstLocalPlayerController(GetWorld()))
    {
        FVector PlayerLocation = GEngine->GetFirstLocalPlayerController(GetWorld())->GetPawn()->GetActorLocation();
        FVector CharacterLocation = GetActorLocation();
        FVector DirectionToPlayer = PlayerLocation - CharacterLocation;
        FRotator NewRotation = DirectionToPlayer.Rotation();
        NewRotation.Pitch = 0.f;
        NewRotation.Roll = 0.f;
        NewRotation.Yaw +=180.f;
        SetActorRotation(NewRotation);
        FVector CharacterForward = GetActorForwardVector();
        float DotProduct = FVector::DotProduct(CharacterForward, DirectionToPlayer.GetSafeNormal());
        if (DotProduct < 0.f)
        {
            // Rotate Y-axis by 180 degrees
            NewRotation.Yaw += 180.f;
        }
    }
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
void ALancer::ApplyDamage(float DamageAmount,float DeltaTime)
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
void ALancer::StartJumpByGravity(float JumpHeight, float GravityAcceleration)
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
void ALancer::JumpingByGravity(float DeltaTime)
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
void ALancer::EndJumpByGravity()
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
void ALancer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}






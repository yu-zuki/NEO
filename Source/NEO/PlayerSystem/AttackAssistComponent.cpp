// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAssistComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/PlayerSystem/CharacterCamera.h"
#include "Camera/CameraComponent.h"
#include "NEO/GameSystem/TGS_GameMode.h"

#define DIRECTION_Y (90.f)

// Sets default values for this component's properties
UAttackAssistComponent::UAttackAssistComponent()
	: bUseCorrectAttackAngle(true)
	, bUseHitStop(true)
	, bUseHitEffect(true)
	, bUseFaceCamera(true)
{
	PrimaryComponentTick.bCanEverTick = false;

	LineLength = 300.f;
	SpeedDuringHitStop = 0.1f;
	HitStopTime = 0.2f;
}


// Called when the game starts
void UAttackAssistComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UAttackAssistComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


/*
 * �֐����@�@�@�@�FCorrectAttackAngle()
 * �������e�@�@�@�F�p�x��␳���čU���𓖂���₷������(������ɂ���G)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UAttackAssistComponent::CorrectAttackAngle()
{
	// �@�\�̃I���E�I�t
	if (!bUseCorrectAttackAngle) { return; }

	//�@�O���ɂ���G�l�~�[�擾
	AActor* InFrontOfEnemy = GetFrontActor();

	if (InFrontOfEnemy)
	{
		// �v���C���[�ƓG�̕������擾����
		FVector direction = InFrontOfEnemy->GetActorLocation() - GetOwner()->GetActorLocation();
		direction.Normalize();

		// �v���C���[�̌������擾����
		FRotator CurrentRotation = GetOwner()->GetActorRotation();

		// �v���C���[�̌�����ύX����
		FRotator NewRotation(CurrentRotation.Pitch, direction.Rotation().Yaw, CurrentRotation.Roll);
		GetOwner()->SetActorRotation(NewRotation);
	}
}


/*
 * �֐����@�@�@�@�FHitStop()
 * �������e�@�@�@�F�q�b�g�X�g�b�v���N����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UAttackAssistComponent::HitStop()
{
	// �@�\�̃I���E�I�t
	if (!bUseHitStop) { return; }

	//�������e
	ACharacter* Character = Cast<ACharacter>(GetOwner());		//CharacterGet
	if (Character == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AttackAssitComponent: Character is nullptr"));
		return;
	}

	//HitStop���J�n
	Character->GetMesh()->GlobalAnimRateScale = SpeedDuringHitStop;

	//HitStop���~
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle_HitStop, this, &UAttackAssistComponent::EndHitStop, HitStopTime, false);
}


/*
 * �֐����@�@�@�@�FSpawnHitEffect()
 * �������e�@�@�@�F�q�b�g�G�t�F�N�g���X�|�[��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UAttackAssistComponent::SpawnHitEffect()
{
	// �@�\�̃I���E�I�t
	if (!bUseHitEffect) { return; }


}

/*
 * �֐����@�@�@�@�FGetFrontActor()
 * �������e�@�@�@�F�G��������ɂ��邩����
 * �߂�l�@�@�@�@�F�������G�̏���Ԃ�
 */
AActor* UAttackAssistComponent::GetFrontActor()
{
	// ���L�҂̏��擾
	AActor* pOwner = GetOwner();

	if (pOwner)
	{
		// ���C���΂�
		// ��΂������w��
		float Rotation_Z = pOwner->GetActorRotation().Yaw;
		float LineDirection = (Rotation_Z > 0) ? (LineLength) : (-LineLength);

		// �n�_
		FVector start = pOwner->GetActorLocation();

		// �I�_
		FVector end = FVector(start.X, start.Y + LineDirection, start.Z);

		// ���g������
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(pOwner);

		// ���������I�u�W�F�N�g�i�[�p
		FHitResult OutHit;

		// �q�b�g�����ꍇtrue
		bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, start, end, ECC_WorldStatic, CollisionParams);
		//DrawDebugLine(GetWorld(), start, end, FColor::Green, true, 1.0f);

		if (isHit)
		{
			AActor* HitEnemy = OutHit.GetActor();

			// "Enemy"�^�O�������Ă���Actor�݂̂�Ԃ�
			if (HitEnemy->ActorHasTag("Enemy"))
			{
				return HitEnemy;
			}
		}
	}

	return nullptr;
}


/*
 * �֐����@�@�@�@�FEndHitStop()
 * �������e�@�@�@�F�q�b�g�X�g�b�v�I��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UAttackAssistComponent::EndHitStop()
{
	//CharacterGet
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AttackAssitComponent: Character is nullptr"));
		return;
	}

	//HitStop���~
	Character->GetMesh()->GlobalAnimRateScale = 1.f;
}


/*
 * �֐����@�@�@�@�FFaceCamera()
 * ����         �Fbool _lookRight�E�E�E���݉E�������Ă��邩
 * �������e�@�@�@�F�I�[�i�[���J�����Ɍ�����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UAttackAssistComponent::FaceCamera(bool _lookRight)
{
	// �@�\�̃I���E�I�t
	if (!bUseFaceCamera) { return; }

	// �Q�[�����[�h�쐬
	ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode) { return; }


	// �J�����̌��݈ʒu�Ɗp�x���擾
	const FVector CameraLocation = GameMode->GetCameraLocation();



	// �J�����̈ʒu���ς���Ă���ΐV�����������v�Z
	if (BeforeCameraPos == CameraLocation) { return; }


	// �I�[�i�[���J�����Ɍ�����
	FVector CameraAngle = GetOwner()->GetActorLocation() - GameMode->GetCameraLocation();
	CameraAngle.Y = 0;
	CameraAngle.Z = 0;

	// ��]�擾
	FRotator LookAtRotation = CameraAngle.Rotation();

	// �J�����̉�]�擾
	const FRotator CameraRotation = GameMode->GetCameraRotation();

	// �I�[�i�[���J�����ƕ��s�ɂ���
	double CameraPitch = CameraRotation.Pitch;
	double CameraYaw = CameraRotation.Yaw;

	// �p�x�␳
	if (!_lookRight)
	{
		LookAtRotation.Roll = CameraPitch;
		LookAtRotation.Yaw = CameraYaw + DIRECTION_Y;

	}
	else
	{
		LookAtRotation.Roll = -CameraPitch;
		LookAtRotation.Yaw = CameraYaw - DIRECTION_Y;

	}

	// ��]
	GetOwner()->SetActorRotation(LookAtRotation);

	// �J�����̏ꏊ��ۑ�
	BeforeCameraPos = CameraLocation;
}

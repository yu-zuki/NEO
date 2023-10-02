// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionAssistComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "NiagaraComponent.h"									
#include "NiagaraFunctionLibrary.h"		
#include "NEOPlayerController.h"

#define DIRECTION_Y (90.f)

// Sets default values for this component's properties
UActionAssistComponent::UActionAssistComponent()
	: bUseCorrectAttackAngle(true)
	, bUseHitStop(true)
	, bUseHitEffect(true)
	, bUseFaceCamera(true)
{
	// Tick()�𖈃t���[���Ă΂Ȃ��悤�ɂ���
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UActionAssistComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UActionAssistComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


/*
 * �֐����@�@�@�@�FHitStop()
 * �����P�@�@�@�@�Ffloat _speedDuringHitStop�E�E�E�~�܂��Ă���Ƃ��̃X�s�[�h(1.f�Œʏ�̑���)
 * �����Q�@�@�@�@�Ffloat _stopTime�E�E�E�~�߂鎞��
 * �������e�@�@�@�F�q�b�g�X�g�b�v���N����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UActionAssistComponent::HitStop(float _speedDuringHitStop,float _stopTime)
{
	// �@�\�̃I���E�I�t
	if (!bUseHitStop) { return; }

	// �������e
	ACharacter* Character = Cast<ACharacter>(GetOwner());		//CharacterGet
	if (Character == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AttackAssitComponent: Character is nullptr"));
		return;
	}

	// HitStop���J�n
	Character->GetMesh()->GlobalAnimRateScale = _speedDuringHitStop;

	// HitStop���~
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle_HitStop, this, &UActionAssistComponent::EndHitStop, _stopTime, false);
}


/*
 * �֐����@�@�@�@�FSpawnEffect()
 * �����P�@�@�@�@�FUNiagaraSystem* _hitParticle�E�E�E�o��������G�t�F�N�g
 * �����Q�@�@�@�@�FFVector _spawnPos�E�E�E�E�E�E�E�E�E�o��������ꏊ
 * �������e�@�@�@�F�G�t�F�N�g���o������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UActionAssistComponent::SpawnEffect(UNiagaraSystem* _hitParticle, FVector _spawnPos)
{
	// �@�\�̃I���E�I�t
	if (!bUseHitEffect) { return; }

	// �G�t�F�N�g�̏�񂪎擾�ł��Ȃ���΃��^�[��
	if (!_hitParticle) { return; }

	//�G�t�F�N�g���o��
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetOwner()->GetWorld(), _hitParticle, _spawnPos);
}


/*
 * �֐����@�@�@�@�FCameraShake()
 * �����P�@�@�@�@�FTSubclassOf<UCameraShakeBase> _shakePattern�E�E�E�h�炷�p�^�[��
 * �����Q�@�@�@�@�Ffloat _scale�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E����
 * �������e�@�@�@�F�J�����V�F�C�N
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UActionAssistComponent::CameraShake(TSubclassOf<UCameraShakeBase> _shakePattern, float _scale /*= 1.f*/)
{
	// �v���C���[�擾
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) { return; }

	// �}�l�[�W���[�擾
	APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
	if (!CameraManager) { return; }

	// �J�����V�F�C�N�J�n
	CameraManager->StartCameraShake(_shakePattern, _scale);
}


/*
 * �֐����@�@�@�@�FPlayAnimation()
 * �������e�@�@�@�F�v���C���[�̃A�j���[�V�����Đ�(�Đ����͑���s��)
 * �����P�@�@�@�@�FUAnimMontage* _toPlayAnimMontage �E�E�E�Đ�����A�j���[�V����
 * �����Q�@�@�@�@�FFName _startSectionName�E�E�E�E�E�E�E�E�R���{�̉��i�ڂ���Đ����邩
 * �����R�@�@�@�@�Ffloat _playRate�E�E�E�E�E�E�E�E�E�E�E�E�A�j���[�V�����̍Đ����x
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UActionAssistComponent::PlayAnimation(UAnimMontage* _toPlayAnimMontage, FName _startSectionName /*= "None"*/, float _playRate /*= 1.f*/)
{
	// �L�����N�^�[�N���X�ɃL���X�g
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (!Owner) { return; }

	// �ړ��s�\��
	//Owner->GetCharacterMovement()->DisableMovement();

	// �Đ�����A�j���[�V�������i�[
	UAnimMontage* toPlayAnimMontage = _toPlayAnimMontage;

	// �A�j���[�V�����Đ�
	if (toPlayAnimMontage != nullptr)
	{
		Owner->PlayAnimMontage(_toPlayAnimMontage, _playRate, _startSectionName);
	}
}


/*
 * �֐����@�@�@�@�FPlaySound()
 * �������e�@�@�@�F�����Đ�
 * �����P�@�@�@�@�FUSoundBase* _sound_Obj�E�E�E�����̃A�Z�b�g
 * �����Q�@�@�@�@�F�������J�n���鎞��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UActionAssistComponent::PlaySound(USoundBase* _sound_Obj,float _startTime /*= 0*/)
{
	if (_sound_Obj)
	{
		// �Đ�(7�Ԗڂ̈������ŃI�[�i�[�A�N�^�[��������)
		UGameplayStatics::PlaySound2D(GetWorld(), _sound_Obj, 1.0f, 1.0f, _startTime, nullptr, GetOwner());
	}
}


/*
 * �֐����@�@�@�@�FEndHitStop()
 * �������e�@�@�@�F�q�b�g�X�g�b�v�I��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UActionAssistComponent::EndHitStop()
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
 * �֐����@�@�@�@�FOwnerParallelToCamera()
 * ����			 �Fbool _lookRight�E�E�E���݉E�������Ă��邩
 * �������e�@�@�@�F�I�[�i�[���J�����Ɍ�����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UActionAssistComponent::OwnerParallelToCamera(bool _lookRight)
{
	// �@�\�̃I���E�I�t
	if (!bUseFaceCamera) { return; }

	// �v���C���[�R���g���[���[�擾
	ANEOPlayerController* PlayerController = Cast<ANEOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));;
	if (!PlayerController) { return; }


	// �J�����̊p�x���擾
	const FRotator CameraRotation = PlayerController->GetNowCameraRotation();


	// �J�����̃s�b�`�ƃ��[
	double CameraPitch = CameraRotation.Pitch;
	double CameraYaw = CameraRotation.Yaw;

	// �p�x�␳
	if (!_lookRight)
	{
		CameraYaw += DIRECTION_Y;

	}
	else
	{
		CameraPitch = -CameraPitch;
		CameraYaw -= DIRECTION_Y;
	}

	// �V�����p�x
	FRotator OwnerAngle = FRotator(0.0, CameraYaw, CameraPitch);

	// ��]
	GetOwner()->SetActorRotation(OwnerAngle);
}


/*
 * �֐����@�@�@�@�FWallCheck()
 * ����			 �Ffloat _lineLength�E�E�E�g���[�X���s������
 * �������e�@�@�@�F�I�[�i�[�̌����Ă�������Ƀ��C���΂��āA��Q�������邩����
 * �߂�l�@�@�@�@�F��Q���̂���Ȃ�
 */
bool UActionAssistComponent::WallCheck(float _lineLength)
{
	// �I�[�i�[�擾
	AActor* pOwner = GetOwner();

	if (!pOwner) { return false; }

	// ���C�L���X�g�����s����ۂ̃p�����[�^��ݒ肷��
	// ���C�L���X�g�̊J�n�ʒu�̓L�����N�^�[�̌��݈ʒu
	float Rotation_Z = pOwner->GetActorRotation().Yaw;
	float LineLength = (Rotation_Z > 0) ? (_lineLength) : (-_lineLength);

	// �n�_
	FVector start = pOwner->GetActorLocation();
	// �I�_
	FVector end = FVector(start.X, start.Y + LineLength, start.Z);


	FCollisionQueryParams CollisionParams;
	// �I�[�i�[���g�͏��O
	CollisionParams.AddIgnoredActor(pOwner);

	FHitResult HitResult;

	// ���C�g���[�X���s��
	bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, start, end, ECC_WorldStatic, CollisionParams);


	return IsHit;
}

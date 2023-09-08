// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionAssistComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/PlayerSystem/CharacterCamera.h"
#include "Camera/CameraComponent.h"
#include "NEO/GameSystem/TGS_GameMode.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include "NiagaraComponent.h"									
#include "NiagaraFunctionLibrary.h"		

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

	RayLength_CorrectAngle = 300.f;
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
 * �֐����@�@�@�@�FCorrectAttackAngle()
 * �������e�@�@�@�F�p�x��␳���čU���𓖂���₷������(������ɂ���G)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UActionAssistComponent::CorrectAttackAngle()
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
 * �֐����@�@�@�@�FGetFrontActor()
 * �������e�@�@�@�F�G��������ɂ��邩����
 * �߂�l�@�@�@�@�F�������G�̏���Ԃ�
 */
AActor* UActionAssistComponent::GetFrontActor()
{
	// ���L�҂̏��擾
	AActor* pOwner = GetOwner();

	if (pOwner)
	{
		// ���C���΂�
		// ��΂������w��
		float Rotation_Z = pOwner->GetActorRotation().Yaw;
		float LineDirection = (Rotation_Z > 0) ? (RayLength_CorrectAngle) : (-RayLength_CorrectAngle);

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

	// �Q�[�����[�h�쐬
	ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode) { return; }


	// �J�����̊p�x���擾
	const FRotator CameraRotation = GameMode->GetCameraRotation();


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


// �ǂƂ̃��C�L���X�g���s���֐�
bool UActionAssistComponent::WallCheck(float _lineLength)
{

	ACharacter* pOwner = Cast<ACharacter>(GetOwner());
	if (!pOwner) { return false; }

	// �J�v�Z���R���|�[�l���g���擾����
	UCapsuleComponent* Capsule = pOwner->GetCapsuleComponent();


	// ���C�L���X�g�����s����ۂ̃p�����[�^��ݒ肷��
	// ���C�L���X�g�̊J�n�ʒu�̓L�����N�^�[�̌��݈ʒu
	float Rotation_Z = pOwner->GetActorRotation().Yaw;
	float LineLength = (Rotation_Z > 0) ? (_lineLength) : (-_lineLength);

	// �n�_
	FVector start = pOwner->GetActorLocation();
	// �I�_
	FVector end = FVector(start.X, start.Y + LineLength, start.Z);

	//UE_LOG(LogTemp, Warning, TEXT("capsuleHeigth : %f"), Capsule->GetScaledCapsuleHalfHeight());
	//UE_LOG(LogTemp, Warning, TEXT("capsuleHeigth / 2 : %f"), Capsule->GetScaledCapsuleHalfHeight() / 2.0f);

	//UE_LOG(LogTemp, Warning, TEXT("StartLocation : %f"), StartLocation.Z);
	//UE_LOG(LogTemp, Warning, TEXT("EndLocation : %f"), EndLocation.Z);

	FCollisionQueryParams TraceParams(FName(TEXT("GroundTrace")), true, GetOwner());
	// ���C�L���X�g�̖�������͊ȈՓI�ɍs��
	TraceParams.bTraceComplex = false;
	// �����}�e���A���̏��͕s�v
	TraceParams.bReturnPhysicalMaterial = false;


	// �J�v�Z���̑傫�����擾����
	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule
	(Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight());

	FHitResult HitResult;
	// ���C�g���[�X���s��
	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, start, end, FQuat::Identity, ECC_Visibility, CapsuleShape, TraceParams);



	return bHit;
}

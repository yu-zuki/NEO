// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAssistComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UAttackAssistComponent::UAttackAssistComponent()
	: bUseCorrectAttackAngle(true)
	, bUseHitStop(true)
	, bUseHitEffect(true)
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

// �U���̊p�x�C��
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

// �q�b�g�X�g�b�v����
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

// �q�b�g�G�t�F�N�g
void UAttackAssistComponent::HitEffect()
{
	// �@�\�̃I���E�I�t
	if (!bUseHitEffect) { return; }



}

// �G�t�F�N�g�X�|�[��
void UAttackAssistComponent::SpawnHitEffect()
{

}

// �O���ɂ���Actor��Ԃ�
AActor* UAttackAssistComponent::GetFrontActor()
{
	// ���C���΂�
	// ��΂������w��
	float Rotation_Z = GetOwner()->GetActorRotation().Yaw;
	float LineDirection = (Rotation_Z > 0) ? (LineLength) : (-LineLength);

	// �n�_
	FVector start = GetOwner()->GetActorLocation();

	// �I�_
	FVector end = FVector(start.X, start.Y + LineDirection, start.Z);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	// ���������I�u�W�F�N�g�i�[�p
	FHitResult OutHit;

	// �q�b�g�����ꍇtrue
	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, start, end, ECC_WorldStatic, CollisionParams);

	DrawDebugLine(GetWorld(),start,end, FColor::Green, true, 1.0f);

	AActor* HitEnemy = OutHit.GetActor();

	if (isHit && HitEnemy != nullptr)
	{
		return HitEnemy;
	}

	return nullptr;
}

// �q�b�g�X�g�b�v�I��
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSword.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/PlayerSystem/PlayerCharacter.h"
#include "NEO/Enemy/EnamyBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APlayerSword::APlayerSword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	// ����̃��b�V���̃p�X
	TCHAR* WeaponAssetPath = TEXT("/Game/0122/Player/Weapon/Sword/Sword");

	// ����̃��b�V���ݒ�
	SetupWeaponMesh(WeaponStaticMesh, WeaponAssetPath);

	// �R���W�����ݒ�
	SetupCollisionComponent(WeaponCollision);

	// ����̎�ނ𓁂ɐݒ�
	WeaponType = EWeaponType::WeaponType_Sword;
}

// Called when the game starts or when spawned
void APlayerSword::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlayerSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/*
 * �֐����@�@�@�@�FSetCollision()
 * �������e�@�@�@�F�v���C���[�̍U���̓����蔻��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerSword::SetCollision()
{
	// �����Ă��Ȃ��Ƃ��̓X�L�b�v
	if (!GetIsHeld()) { return; }

	switch (OwnerType)
	{
	case EOwnerType::OwnerType_Player:
		PlyerAttack();
		break;
	case EOwnerType::OwnerType_Enemy:
		EnemyAttack();
		break;
	case EOwnerType::OwnerType_Boss:
		BossAttack();
		break;
	}
}

// �v���C���[�̓����蔻��
void APlayerSword::PlyerAttack()
{
	// �v���C���[�̃x�[�X�N���X�ɃL���X�g
	APlayerBase* Player = Cast<APlayerCharacter>(pOwner);

	if (Player)
	{
		// �����蔻������͈�
		FVector Start = WeaponCollision->GetComponentLocation();
		FVector End = Start - 50.f;

		// ���������I�u�W�F�N�g�i�[�p
		TArray<FHitResult> HitResults;

		// �U�����������Ă��邩����
		bool IsHit = GetHitResults(Start, End, HitResults);

		if (IsHit)
		{
			// ���������I�u�W�F�N�g�̐������J��Ԃ�
			for (const FHitResult HitResult : HitResults)
			{
				// ���������L�����N�^�[���i�[
				AActor* tempActor = HitResult.GetActor();

				// �v���C���[���猻�݂̍U���͂��擾
				float DamageAmount = Player->GetDamageAmount();

				// ��ɃI�u�W�F�N�g�ɓ��������珈�����Ȃ�
				if (tempActor && tempActor->ActorHasTag("Object"))
				{
					// �I�u�W�F�N�g�ɍU��
					AttackObject(tempActor, DamageAmount, ObjectHitSoundObj);
					break;
				}

				// �q�b�g�����A�N�^�[��"Enemy"�^�O�������Ă�����
				if (tempActor && tempActor->ActorHasTag("Enemy"))
				{
					// �G�ɍU��
					AttackEnemy(tempActor,DamageAmount, EnemyHitSoundObj);

					if (HitEffect)
					{
						// �q�b�g�G�t�F�N�g�\��
						ActionAssistComp->SpawnEffect(HitEffect, HitResult.Location);
					}
					break;
				}
			}
		}
	}
}

// �G�̓����蔻��
void APlayerSword::EnemyAttack()
{
	AEnamyBase* pEnemy = Cast<AEnamyBase>(pOwner);

	if (pEnemy)
	{
		// �����蔻������͈�
		FVector Start = WeaponCollision->GetComponentLocation();
		FVector End = Start;

		// ���������I�u�W�F�N�g�i�[�p
		TArray<FHitResult> HitResults;

		// �U�����������Ă��邩����
		bool IsHit = GetHitResults(Start, End, HitResults);

		if (IsHit)
		{
			// �_���[�W��
			float DamageAmount = 5.f;
			// ���������I�u�W�F�N�g�̐������J��Ԃ�
			for (const FHitResult HitResult : HitResults)
			{
				// ���������L�����N�^�[���i�[
				AActor* tempActor = HitResult.GetActor();

				// �q�b�g�����A�N�^�[��"Enemy"�^�O�������Ă�����
				if (tempActor && tempActor->ActorHasTag("Player"))
				{

					// Player�̃_���[�W����
					APlayerBase* Player = Cast<APlayerBase>(HitResult.GetActor());

					if (Player)
					{
						Player->TakedDamage(DamageAmount);

						pEnemy->ActionAssistComp->HitStop(0.1f, HitStopTime);

						if (EnemyHitSoundObj)
						{
							// �a��SE�Đ�
							ActionAssistComp->PlaySound(EnemyHitSoundObj);
						}
					}

					break;
				}
			}
		}
	}

}


/*
 * �֐����@�@�@�@�FSetHitStopTime()
 * �������e�@�@�@�F�G�ɍU���������̃q�b�g�X�g�b�v�̎��Ԃ�ݒ�
 * �߂�l�@�@�@�@�F�q�b�g�X�g�b�v�̎��Ԃ�Ԃ�
 */
float APlayerSword::SetHitStopTime(int _comboNum)
{
	// �����ݒ�
	float hitStopTime = HitStopTime;

	// �R���{�ŏI�i�ɂ�A�q�b�g�X�g�b�v����������

	for (int i = 2; i < _comboNum; ++i)
	{
		if (_comboNum)
		{
			hitStopTime += HitStopTimeRise;
		}
	}

	return hitStopTime;
}

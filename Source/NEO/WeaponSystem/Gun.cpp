// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/PlayerSystem/PlayerCharacter.h"
#include "NEO/Enemy/EnamyBase.h"
#include "NEO/OdaBase.h"
#include "NEO/BackGroundSystem/ObjectBase.h"
#include "Components/CapsuleComponent.h"
#include "PlayerBullet.h"


// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	// ����̃��b�V���̃p�X
	TCHAR* WeaponAssetPath = TEXT("/Game/0122/Player/Weapon/Gun/Gun");

	// ����̃��b�V���ݒ�
	SetupWeaponMesh(WeaponStaticMesh, WeaponAssetPath);

	// �R���W�����ݒ�
	SetupCollisionComponent(WeaponCollision);

	// ����̎�ނ𓁂ɐݒ�
	WeaponType = EWeaponType::WeaponType_Gun;
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/*
 * �֐����@�@�@�@�FSetCollision()
 * �������e�@�@�@�F�v���C���[�̍U���̓����蔻��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AGun::SetCollision()
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
void AGun::PlyerAttack()
{
	// �v���C���[�̃x�[�X�N���X�ɃL���X�g
	APlayerBase* Player = Cast<APlayerCharacter>(pOwner);

	if (Player)
	{
		// �R��U�����ǂ���
		const bool Kicking = Player->GetKicking();

		if (!Kicking)
		{
			// �R��U������Ȃ����e�𔭎�
			SpawnBullet();
		}
		else
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
					float DamageAmount = 10.f;

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
						AttackEnemy(tempActor, DamageAmount, EnemyHitSoundObj);

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
}

// �G�̓����蔻��
void AGun::EnemyAttack()
{
	// �G�͔��˂̂�
	SpawnBullet();
}

// �G�ɍU���������̏���
void AGun::AttackEnemy(AActor* _enemy, float _damageAmount, class USoundBase* _hitSoundObj)
{
	// �v���C���[�擾
	APlayerBase* Player = Cast<APlayerBase>(pOwner);

	// �G�l�~�[��damage����
	AEnamyBase* Enemy = Cast<AEnamyBase>(_enemy);
	AOdaBase* Boss = Cast<AOdaBase>(_enemy);

	// �q�b�g�X�g�b�v
	Player->HitStop(0.1f, HitStopTime);

	if (Enemy)
	{
		// �G�̃_���[�W����
		Enemy->ApplyDamage(_damageAmount);

		// �G�̃m�b�N�o�b�N����
		if (!Enemy->ActionAssistComp->WallCheck(KnockBackDistance))
		{
			Enemy->AddActorLocalOffset(FVector(KnockBackDistance, 0.f, 0.f));
		}

		// �G����ɓ����Ă���̂�
		// �R���{��������₷���悤�ɓG�ɂ��~�܂��Ă��炤
		Enemy->ActionAssistComp->HitStop(0.1f, HitStopTime * 2.f);
	}
	else if (Boss)
	{
		// �{�X�̃_���[�W����
		Boss->ApplyDamage(_damageAmount);

		// �G�̃m�b�N�o�b�N����
		if (!Boss->ActionAssistComp->WallCheck(KnockBackDistance))
		{
			Boss->AddActorLocalOffset(FVector(KnockBackDistance, 0.f, 0.f));
		}

		// �G����ɓ����Ă���̂�
		// �R���{��������₷���悤�ɓG�ɂ��~�܂��Ă��炤
		Boss->ActionAssistComp->HitStop(0.1f, HitStopTime * 2.f);
	}

	if (_hitSoundObj)
	{
		// �R��SE�Đ�
		ActionAssistComp->PlaySound(_hitSoundObj);
	}
}



// �e�e���X�|�[��
void AGun::SpawnBullet()
{
	// �e�e���X�|�[��
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// �X�|�[�����̏ꏊ�Ɖ�]���w��
	FVector SpawnBulletLocation = GetActorLocation();
	FRotator SpawnBulletRotation = pOwner->GetActorRotation();

	GetWorld()->SpawnActor<AActor>(BulletClass, SpawnBulletLocation, SpawnBulletRotation, SpawnParams);

	if (ShootSoundObj)
	{
		// ���ˉ��Đ�
		ActionAssistComp->PlaySound(ShootSoundObj);
	}
}

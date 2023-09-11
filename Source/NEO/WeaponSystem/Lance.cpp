// Fill out your copyright notice in the Description page of Project Settings.


#include "Lance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/PlayerSystem/PlayerCharacter.h"
#include "NEO/Enemy/EnamyBase.h"
#include "NEO/OdaBase.h"
#include "NEO/BackGroundSystem/ObjectBase.h"
#include "Components/CapsuleComponent.h"
#include "NEO/PlayerSystem/ActionAssistComponent.h"

// Sets default values
ALance::ALance()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	// ����̃��b�V���̃p�X
	TCHAR* WeaponAssetPath = TEXT("/Game/0122/Player/Weapon/Lance/Lance");

	// ����̃��b�V���ݒ�
	SetupWeaponMesh(WeaponStaticMesh, WeaponAssetPath);

	// �R���W�����ݒ�
	SetupCollisionComponent(WeaponCollision);

	// ����̎�ނ𓁂ɐݒ�
	WeaponType = EWeaponType::WeaponType_Lance;

	// �A�^�b�N�A�V�X�g�R���|�[�l���g�쐬
	ActionAssistComp = CreateDefaultSubobject<UActionAssistComponent>(TEXT("AttackAssist"));
}

// Called when the game starts or when spawned
void ALance::BeginPlay()
{
	Super::BeginPlay();


	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(this, 0);

	SetupOwnerData(pPlayer, "Player", "hand_rSocket");
}

// Called every frame
void ALance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/*
 * �֐����@�@�@�@�FSetCollision()
 * �������e�@�@�@�F�v���C���[�̍U���̓����蔻��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ALance::SetCollision()
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
void ALance::PlyerAttack()
{
	// �v���C���[�̃x�[�X�N���X�ɃL���X�g
	APlayerBase* pPlayer = Cast<APlayerCharacter>(OwnerInfo.pOwner);

	if (pPlayer)
	{
		// �����ƃv���C���[�ɓ�����Ȃ��悤�ɂ���
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		CollisionParams.AddIgnoredActor(pPlayer);

		TArray<FHitResult> HitResults;

		float DamageAmount = pPlayer->GetDamageAmount();

		// �����蔻������͈�
		FVector Start = WeaponCollision->GetComponentLocation();
		FVector End = Start;
		FQuat Rot = WeaponCollision->GetComponentQuat();
		FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(WeaponCollision->GetScaledCapsuleRadius(), WeaponCollision->GetScaledCapsuleHalfHeight());

		// �������Ă��邩�m�F
		bool isHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, Rot, ECollisionChannel::ECC_GameTraceChannel1, CollisionShape, CollisionParams);

		if (isHit)
		{
			// ���������I�u�W�F�N�g�̐������J��Ԃ�
			for (const FHitResult HitResult : HitResults)
			{
				// ���������L�����N�^�[���i�[
				AActor* tempActor = HitResult.GetActor();

				// ��ɃI�u�W�F�N�g�ɓ��������珈�����Ȃ�
				if (tempActor && tempActor->ActorHasTag("Object"))
				{
					AObjectBase* Object = Cast<AObjectBase>(HitResult.GetActor());

					if (Object)
					{
						Object->ReceiveDamage(DamageAmount);

						// �I�u�W�F�N�g�j��p�̃T�E���h�Đ�
						ActionAssistComp->PlaySound(ObjectHitSoundObj);
					}
				}

				// �q�b�g�����A�N�^�[��"Enemy"�^�O�������Ă�����
				if (tempActor && tempActor->ActorHasTag("Enemy"))
				{

					// �G�l�~�[��damage����
					AEnamyBase* Enemy = Cast<AEnamyBase>(HitResult.GetActor());
					AOdaBase* Oda = Cast<AOdaBase>(HitResult.GetActor());

					int ComboNum = pPlayer->GetComboIndex();

					if (ComboNum < 2)
					{
						HitStopTime = 0.2f;
					}
					else if (ComboNum == 2)
					{
						HitStopTime = 0.25f;
					}
					else
					{
						HitStopTime = 0.3f;
					}


					// �q�b�g�X�g�b�v
					pPlayer->HitStop(0.1f, HitStopTime);

					// �R���{�̃t�B�j�b�V���̂݃J������h�炷
					if (ComboNum == 3)
					{
						pPlayer->CameraShake();
					}

					if (EnemyHitSoundObj)
					{
						// �a��SE�Đ�
						ActionAssistComp->PlaySound(EnemyHitSoundObj);
					}

					if (Enemy)
					{
						Enemy->ApplyDamage(DamageAmount);

						if (ComboNum == 1)
						{
							Enemy->AddActorLocalOffset(FVector(-100.f, 0.f, 0.f));
						}
						else if (ComboNum == 2)
						{
							Enemy->AddActorLocalOffset(FVector(-100.f, 0.f, 0.f));
						}

						Enemy->ActionAssistComp->HitStop(0.1f, HitStopTime);
					}
					else if (Oda)
					{
						Oda->ApplyDamage(DamageAmount);

						if (pPlayer->GetComboIndex() == 3)
						{
							Oda->BossKnockback();
						}
					}
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
void ALance::EnemyAttack()
{
	AEnamyBase* pEnemy = Cast<AEnamyBase>(OwnerInfo.pOwner);

	if (pEnemy)
	{
		// �����ƃv���C���[�ɓ�����Ȃ��悤�ɂ���
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		CollisionParams.AddIgnoredActor(pEnemy);

		TArray<FHitResult> HitResults;

		// �����蔻������͈�
		FVector Start = WeaponCollision->GetComponentLocation();
		FVector End = Start;
		FQuat Rot = WeaponCollision->GetComponentQuat();
		FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(WeaponCollision->GetScaledCapsuleRadius(), WeaponCollision->GetScaledCapsuleHalfHeight());

		// �������Ă��邩�m�F
		bool isHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, Rot, ECollisionChannel::ECC_GameTraceChannel1, CollisionShape, CollisionParams);

		if (isHit)
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

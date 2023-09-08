// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSword.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/PlayerSystem/PlayerCharacter.h"
#include "NEO/Enemy/EnamyBase.h"
#include "NEO/OdaBase.h"
#include "NEO/BackGroundSystem/ObjectBase.h"
#include "Components/CapsuleComponent.h"
#include "NEO/PlayerSystem/ActionAssistComponent.h"

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

}

// Called when the game starts or when spawned
void APlayerSword::BeginPlay()
{
	Super::BeginPlay();


	AActor* pPlayer = UGameplayStatics::GetPlayerCharacter(this, 0);

	SetupOwnerData(pPlayer, "Player", "hand_rSocket");
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
							Enemy->AddActorLocalOffset(FVector(-300.f, 0.f, 0.f));
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

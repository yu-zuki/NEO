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
	APlayerBase* pPlayer = Cast<APlayerCharacter>(pOwner);

	if (pPlayer)
	{
		// �R��U�����ǂ���
		const bool Kicking = pPlayer->GetKicking();

		if (!Kicking)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FVector SpawnBulletLocation = GetActorLocation();
			FRotator SpawnBulletRotation = pOwner->GetActorRotation();

			GetWorld()->SpawnActor<AActor>(BulletClass, SpawnBulletLocation, SpawnBulletRotation, SpawnParams);
		}
		else
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

						if (Enemy)
						{
							Enemy->ApplyDamage(10.f);

							// �m�b�N�o�b�N
							Enemy->AddActorLocalOffset(FVector(-50.f, 0.f, 0.f));
						}
						else if (Oda)
						{
							Oda->ApplyDamage(10.f);
							Oda->BossKnockback();
						}
					}
				}
			}
		}
	}
}

// �G�̓����蔻��
void AGun::EnemyAttack()
{
	FVector SpawnBulletLocation = GetActorLocation(); // �܂��͑��̏ꏊ
	FRotator SpawnBulletRotation = pOwner->GetActorRotation(); // �܂��͑��̉�]
	if (BulletClass)
	{
		AActor* SpawnedBullet = GetWorld()->SpawnActor<AActor>(BulletClass, SpawnBulletLocation, SpawnBulletRotation);
	}
}



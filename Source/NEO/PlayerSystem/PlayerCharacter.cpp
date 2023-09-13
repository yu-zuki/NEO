// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Player�̃Z�b�g�A�b�v
	SetupPlayerData();

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


/*
 * �֐����@�@�@�@�FSetupPlayerData()
 * �������e�@�@�@�F�v���C���[�̃f�[�^������(APlayerBase����I�[�o�[���C�h)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerCharacter::SetupPlayerData()
{
	Super::SetupPlayerData();

	//// ����̃��b�V���̃p�X
	//TCHAR* WeaponAssetPath = TEXT("/Game/0139/CharacterModel/EnemySwordJoint");

	//// ����̃��b�V���ݒ�
	//SetupWeaponMesh(WeaponStaticMesh, WeaponAssetPath);

	//// �R���W�����ݒ�
	//SetupCollisionComponent(WeaponCollision);

	// �A�j���[�V�����A�Z�b�g�ݒ�
	SetupAnimationAsset();
}


/*
 * �֐����@�@�@�@�FSetupAnimationAsset()
 * �������e�@�@�@�F�A�j���[�V�����A�Z�b�g�̃Z�b�g�A�b�v
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerCharacter::SetupAnimationAsset()
{
	// �R���{�A�j���[�V�����̃p�X�ۊǗp
	{
		TCHAR* ComboAnimationAssetPaths[2];

		// �A�j���[�V�����A�Z�b�g�̃p�X
		ComboAnimationAssetPaths[0] = TEXT("/Game/0122/Player/Animation/Montage/Combo/Combo1");
		ComboAnimationAssetPaths[1] = TEXT("/Game/0122/Player/Animation/Montage/Combo/Combo2");

		for (int i = 0; i < 2; ++i)
		{
			PlayerAnimation.ComboAttack[i] = GetAnimationAsset(ComboAnimationAssetPaths[i]);
		}
	}
	
	{
		// �󒆂ł̍U���A�j���[�V����
		TCHAR* AirAttackAnimationAssetPath = TEXT("/Game/0122/Player/Animation/Montage/Combo/JumpAttack_Montage");

		PlayerAnimation.AirAttack = GetAnimationAsset(AirAttackAnimationAssetPath);
	}

	{
		// �󒆂ł̍U���A�j���[�V����
		TCHAR* ChargeAttackAnimationAssetPath = TEXT("/Game/0122/Player/Animation/Montage/ChargeAttack_Montage");

		PlayerAnimation.ChargeAttack = GetAnimationAsset(ChargeAttackAnimationAssetPath);
	}

	{
		// �󒆂ł̍U���A�j���[�V����
		TCHAR* GunAttackAnimationAssetPath = TEXT("/Game/0122/Player/Animation/Montage/GunAttack_Montage");

		PlayerAnimation.GunAttack = GetAnimationAsset(GunAttackAnimationAssetPath);
	}

	{
		// �e���A�j���[�V����
		TCHAR* GunAttack2AnimationAssetPath = TEXT("/Game/0122/Player/Animation/Montage/Kick_Montage");

		PlayerAnimation.GunAttack2 = GetAnimationAsset(GunAttack2AnimationAssetPath);
	}	


	{
		// ��_���[�W�A�j���[�V�����̃p�X�ۊǗp
		TCHAR* DamageAnimationAssetPath = TEXT("/Game/0122/Player/Animation/Montage/Damaged_Montage");

		PlayerAnimation.TakeDamage = GetAnimationAsset(DamageAnimationAssetPath);
	}

	{
		// ��_���[�W�A�j���[�V�����̃p�X�ۊǗp
		TCHAR* KnockBackAnimationAssetPath = TEXT("/Game/0122/Player/Animation/Montage/KnockBack_Montage");

		PlayerAnimation.KnockBack = GetAnimationAsset(KnockBackAnimationAssetPath);
	}

	{
		// ���S���A�j���[�V�����̃p�X�ۊ�
		TCHAR* DeathAnimationAssetPath = TEXT("/Game/0122/Player/Animation/Montage/Death_Montage");

		PlayerAnimation.Death = GetAnimationAsset(DeathAnimationAssetPath);
	}
}


/*
 * �֐����@�@�@�@�FSetCollision()
 * �������e�@�@�@�F�v���C���[�̍U���̓����蔻��(APlayerBase����I�[�o�[���C�h)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerCharacter::SetCollision()
{
	// ����������Ă���Ƃ������蔻�������
	if (Weapon)
	{
		Weapon->SetCollision();
	}
}
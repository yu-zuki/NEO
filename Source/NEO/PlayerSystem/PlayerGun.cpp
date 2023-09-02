// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGun.h"
#include "PlayerBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/StreamableManager.h"
#include "Engine/World.h"
#include "Engine/AssetManager.h"
#include "Async/Async.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerGun::APlayerGun()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �L�����N�^�[�R���|�[�l���g�擾
	CharacterMovementComp = GetCharacterMovement();
	CharacterMovementComp->MaxWalkSpeed = 500.f;

	// �v���C���[�̃f�[�^�Z�b�g
	SetupPlayerData();

}

void APlayerGun::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerGun::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

// �v���C���[�̃f�[�^��������
void APlayerGun::SetupPlayerData()
{
	Super::SetupPlayerData();

	// ����̃��b�V���̃p�X
	TCHAR* WeaponAssetPath = TEXT("/Game/0122/Player/Weapon/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight");

	// �Z�b�g
	SetupWeaponMesh(WeaponSkeletalMesh, WeaponAssetPath, "WeaponMesh");

	SetupCollisionComponent(WeaponCollision);

	// �A�j���[�V�����Z�b�g�A�b�v
	TCHAR* AnimationAssetPaths[2];

	// �A�j���[�V�����A�Z�b�g�̃p�X
	AnimationAssetPaths[0] = TEXT("/Game/0207/GunAnimation/montage/GunCombo_1.GunCombo_1");
	//AnimationAssetPaths[0] = TEXT("/Game/0122/Player/Animation/Montage/Combo/SwordCombo");

	AnimationAssetPaths[1] = TEXT("/Game/0122/Player/Animation/Montage/Combo/SwordCombo2");

	for (int i = 0; i < 2; ++i)
	{
		PlayerAnimation.ComboAttack[i] = GetAnimationAsset(AnimationAssetPaths[i]);
	}
}

// �U��
void APlayerGun::Attack(int AttackNum)
{
	//if (!IsAttacking)
	//{
	//	// �U�����t���O�I��
	//	IsAttacking = true;
	//}
	//else
	//{
	//	// �R���{�\�Ȏ�,�p��
	//	if (CanCombo)
	//	{
	//		// ���X�g�A�^�b�N�܂ŃR���{�p��
	//		if (ComboStartSectionNames[ComboIndex] != ComboStartSectionNames.Last())
	//		{
	//			++ComboIndex;
	//		}
	//	}
	//}

	//// �U���̃A�j���[�V�����Đ�
	//PlayAnimation(ComboAnimMontages[AttackNum], ComboStartSectionNames[ComboIndex]);

}

// �U��
void APlayerGun::Combo1()
{
	//// �R���g���[���\��
	//if (!IsControl) { return; }

	//// �U��
	//Attack(0);

}


// �U��
void APlayerGun::Combo2()
{
	//// �R���g���[���\��
	//if (!IsControl) { return; }

	//// �U��
	//Attack(1);

}
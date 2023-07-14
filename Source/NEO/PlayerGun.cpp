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

	// �v���C���[�̐ݒ�
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// �L�����N�^�[�R���|�[�l���g�擾
	CharacterMovementComp = GetCharacterMovement();
	CharacterMovementComp->MaxWalkSpeed = 500.f;

}

void APlayerGun::BeginPlay()
{

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

	//// ����̃��b�V���̃p�X
	//TCHAR* WeaponAssetPath = TEXT("/Game/0122/Player/Weapon/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight");

	//// �Z�b�g
	//SetupWeaponMesh(WeaponAssetPath, "WeaponMesh");

	//// �A�j���[�V�����Z�b�g�A�b�v
	//TCHAR* AnimationAssetPaths[2];

	//// �A�j���[�V�����A�Z�b�g�̃p�X
	//AnimationAssetPaths[0] = TEXT("/Game/0122/Player/Animation/Montage/Combo/SwordCombo");
	//AnimationAssetPaths[1] = TEXT("/Game/0122/Player/Animation/Montage/Combo/SwordCombo2");

	//SetupAnimationAsset(AnimationAssetPaths);

	//WeaponCollision = SetupCollisionComponent(WeaponCollision);
}


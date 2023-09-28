// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NEOPlayerController.generated.h"

class APlayerBase;
class AWeaponBase;

UCLASS()
class NEO_API ANEOPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// �R���X�g���N�^
	ANEOPlayerController();

	// �Q�[���J�n���̏���
	void BeginPlay();

	// �����ꂩ�̃L�[����͒������擾
	UFUNCTION(BlueprintCallable, Category = "SetStatus")
		bool GetIsAnyKeyPressed()const { return IsInputKeyDown("AnyKey") && GetIsDebugKeyPressed(); }

	UFUNCTION(BlueprintCallable, Category = "SetStatus")
		bool AnyEnemiesNearbyMoreSecond()const;

	// �v���C���[�̏�ԃ��Z�b�g
	void ResetPlayerStatus();

	// �v���C���[�̎c�@�擾
	int GetRemainingLives()const { return RemainingLives; }

	// �v���C���[�̎c�@���炷
	void ReduceRemainingLives() { --RemainingLives; }

	// �v���C���[�̍폜
	void DestroyPlayer();

	// �v���C���[�̃��X�|�[��
	void RespawnPlayer();

	// �v���C���[�̈ʒu�擾
	FVector GetPlayerLocation()const { return pPlayer->GetActorLocation(); }

	// ���݂̃v���C���[�̃J�����擾
	FRotator GetNowCameraRotation()const;

	// �v���C���[������ł��邩��`����
	bool GetPlayerIsDead()const { return PlayerIsDead; }

	// �v���C���[������������Ă��邩�`����
	bool GetPlayerIsHeldWeapon()const { return PlayerIsHeldWeapon; }

	// �v���C���[��������E���鋗���ɂ��邩�ǂ���
	float GetPickUpDistance()const { return PickUpWeaponDistance; }

	// �E���镐���ǉ�
	void AddWeapons(AWeaponBase* _weapon) { CanPickUpWeapons.Add(_weapon); }

	// ���ꂽ������폜
	void RemoveWeapons(AWeaponBase* _weapon);

private:

	// �f�o�b�O�L�[�������ꂽ��
	bool GetIsDebugKeyPressed()const;

	// �o�^���ꂽ����̒��ŋ�������ԋ߂����̂�T��
	AWeaponBase* GetClosestDistanceWeapons()const;


private:

	// �v���C���[�����X�|�[���ł����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int DefaultRemainingLives;

	// �v���C���[�̋߂��ɉ��̓G�������狭���\�����邩
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int EnemiesCnt;

	// �v���C���[�����X�|�[���ł����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float PlayerToEnemyDistance;

	// ���킪�E����悤�ɂȂ鋗��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float PickUpWeaponDistance;

	// PressAny����F�L�[�͏���
	FName KeyNames[10] = { "F1","F2","F3","F4","F5","F6","F7","F8","F9","F10" };

	// �v���C���[�̕����\��
	int RemainingLives;

	// �v���C���[������ł��邩
	bool PlayerIsDead;

	// �v���C���[������������Ă��邩
	bool PlayerIsHeldWeapon;

	// �v���C���[�̏��
	APlayerBase* pPlayer;

	// �Q�[�����[�h
	class ANEOGameMode* pGameMode;

	// ����̏��
	TArray<AWeaponBase*> CanPickUpWeapons;
};


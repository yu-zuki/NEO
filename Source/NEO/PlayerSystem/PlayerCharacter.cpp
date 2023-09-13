// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Playerのセットアップ
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
 * 関数名　　　　：SetupPlayerData()
 * 処理内容　　　：プレイヤーのデータ初期化(APlayerBaseからオーバーライド)
 * 戻り値　　　　：なし
 */
void APlayerCharacter::SetupPlayerData()
{
	Super::SetupPlayerData();

	//// 武器のメッシュのパス
	//TCHAR* WeaponAssetPath = TEXT("/Game/0139/CharacterModel/EnemySwordJoint");

	//// 武器のメッシュ設定
	//SetupWeaponMesh(WeaponStaticMesh, WeaponAssetPath);

	//// コリジョン設定
	//SetupCollisionComponent(WeaponCollision);

	// アニメーションアセット設定
	SetupAnimationAsset();
}


/*
 * 関数名　　　　：SetupAnimationAsset()
 * 処理内容　　　：アニメーションアセットのセットアップ
 * 戻り値　　　　：なし
 */
void APlayerCharacter::SetupAnimationAsset()
{
	// コンボアニメーションのパス保管用
	{
		TCHAR* ComboAnimationAssetPaths[2];

		// アニメーションアセットのパス
		ComboAnimationAssetPaths[0] = TEXT("/Game/0122/Player/Animation/Montage/Combo/Combo1");
		ComboAnimationAssetPaths[1] = TEXT("/Game/0122/Player/Animation/Montage/Combo/Combo2");

		for (int i = 0; i < 2; ++i)
		{
			PlayerAnimation.ComboAttack[i] = GetAnimationAsset(ComboAnimationAssetPaths[i]);
		}
	}
	
	{
		// 空中での攻撃アニメーション
		TCHAR* AirAttackAnimationAssetPath = TEXT("/Game/0122/Player/Animation/Montage/Combo/JumpAttack_Montage");

		PlayerAnimation.AirAttack = GetAnimationAsset(AirAttackAnimationAssetPath);
	}

	{
		// 空中での攻撃アニメーション
		TCHAR* ChargeAttackAnimationAssetPath = TEXT("/Game/0122/Player/Animation/Montage/ChargeAttack_Montage");

		PlayerAnimation.ChargeAttack = GetAnimationAsset(ChargeAttackAnimationAssetPath);
	}

	{
		// 空中での攻撃アニメーション
		TCHAR* GunAttackAnimationAssetPath = TEXT("/Game/0122/Player/Animation/Montage/GunAttack_Montage");

		PlayerAnimation.GunAttack = GetAnimationAsset(GunAttackAnimationAssetPath);
	}

	{
		// 銃撃アニメーション
		TCHAR* GunAttack2AnimationAssetPath = TEXT("/Game/0122/Player/Animation/Montage/Kick_Montage");

		PlayerAnimation.GunAttack2 = GetAnimationAsset(GunAttack2AnimationAssetPath);
	}	


	{
		// 被ダメージアニメーションのパス保管用
		TCHAR* DamageAnimationAssetPath = TEXT("/Game/0122/Player/Animation/Montage/Damaged_Montage");

		PlayerAnimation.TakeDamage = GetAnimationAsset(DamageAnimationAssetPath);
	}

	{
		// 被ダメージアニメーションのパス保管用
		TCHAR* KnockBackAnimationAssetPath = TEXT("/Game/0122/Player/Animation/Montage/KnockBack_Montage");

		PlayerAnimation.KnockBack = GetAnimationAsset(KnockBackAnimationAssetPath);
	}

	{
		// 死亡時アニメーションのパス保管
		TCHAR* DeathAnimationAssetPath = TEXT("/Game/0122/Player/Animation/Montage/Death_Montage");

		PlayerAnimation.Death = GetAnimationAsset(DeathAnimationAssetPath);
	}
}


/*
 * 関数名　　　　：SetCollision()
 * 処理内容　　　：プレイヤーの攻撃の当たり判定(APlayerBaseからオーバーライド)
 * 戻り値　　　　：なし
 */
void APlayerCharacter::SetCollision()
{
	// 武器を持っているとき当たり判定をつける
	if (Weapon)
	{
		Weapon->SetCollision();
	}
}
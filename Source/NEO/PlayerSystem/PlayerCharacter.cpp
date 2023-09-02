// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "NEO/Enemy/EnamyBase.h"
#include "../OdaBase.h"
#include "NEO/BackGroundSystem/ObjectBase.h"
#include "ActionAssistComponent.h"

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

	// 武器のメッシュのパス
	TCHAR* WeaponAssetPath = TEXT("/Game/0139/CharacterModel/EnemySwordJoint");

	// 武器のメッシュ設定
	SetupWeaponMesh(WeaponStaticMesh, WeaponAssetPath);

	// コリジョン設定
	SetupCollisionComponent(WeaponCollision);

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
	// 自身に当たらないようにする
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	TArray<FHitResult> HitResults;

	// 当たり判定を取る範囲
	FVector Start = WeaponCollision->GetComponentLocation();
	FVector End = Start;
	FQuat Rot = WeaponCollision->GetComponentQuat();
	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(WeaponCollision->GetScaledCapsuleRadius(), WeaponCollision->GetScaledCapsuleHalfHeight());

	// あたっているか確認
	bool isHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, Rot, ECollisionChannel::ECC_GameTraceChannel1, CollisionShape, CollisionParams);

	if (isHit)
	{
		// 当たったオブジェクトの数だけ繰り返し
		for (const FHitResult HitResult : HitResults)
		{
			// 当たったキャラクターを格納
			AActor* tempActor = HitResult.GetActor();

			// 先にオブジェクトに当たったら処理しない
			if (tempActor && tempActor->ActorHasTag("Object"))
			{
				AObjectBase* Object = Cast<AObjectBase>(HitResult.GetActor());

				if (Object)
				{
					Object->ReceiveDamage(GetDamageAmount());

				}
				break; 
			}

			// ヒットしたアクターが"Enemy"タグを持っていたら
			if (tempActor && tempActor->ActorHasTag("Enemy"))
			{

				// エネミーのdamage処理
				AEnamyBase* Enemy = Cast<AEnamyBase>(HitResult.GetActor());
				AOdaBase* Oda = Cast<AOdaBase>(HitResult.GetActor());

				if (Enemy)
				{
					// ヒットストップ
					ActionAssistComp->HitStop(HitStopTime);
					Enemy->ApplyDamage(GetDamageAmount());
					
				}
				else if (Oda) 
				{
					// ヒットストップ
					ActionAssistComp->HitStop(HitStopTime);
					Oda->ApplyDamage(GetDamageAmount());

					if (GetComboIndex() == 2)
					{
						Oda->BossKnockback();
					}

				}

				// コンボのフィニッシュのみカメラを揺らす
				if (GetComboIndex() == 2)
				{
					ActionAssistComp->CameraShake(ShakePattern);

				}
			}
		}
	}
}
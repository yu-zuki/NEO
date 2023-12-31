// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSword.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/PlayerSystem/PlayerCharacter.h"
#include "NEO/Enemy/EnamyBase.h"
#include "NEO/OdaBase.h"
#include "NEO/BackGroundSystem/ObjectBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APlayerSword::APlayerSword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	// 武器のメッシュのパス
	TCHAR* WeaponAssetPath = TEXT("/Game/0122/Player/Weapon/Sword/Sword");

	// 武器のメッシュ設定
	SetupWeaponMesh(WeaponStaticMesh, WeaponAssetPath);

	// コリジョン設定
	SetupCollisionComponent(WeaponCollision);

	// 武器の種類を刀に設定
	WeaponType = EWeaponType::WeaponType_Sword;
}

// Called when the game starts or when spawned
void APlayerSword::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlayerSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/*
 * 関数名　　　　：SetCollision()
 * 処理内容　　　：プレイヤーの攻撃の当たり判定
 * 戻り値　　　　：なし
 */
void APlayerSword::SetCollision()
{
	// 持っていないときはスキップ
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

// プレイヤーの当たり判定
void APlayerSword::PlyerAttack()
{
	// プレイヤーのベースクラスにキャスト
	APlayerBase* Player = Cast<APlayerCharacter>(pOwner);

	if (Player)
	{
		// 自分とプレイヤーに当たらないようにする
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		CollisionParams.AddIgnoredActor(Player);

		TArray<FHitResult> HitResults;

		float DamageAmount = Player->GetDamageAmount();

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
						Object->ReceiveDamage(DamageAmount);

						// オブジェクト破壊用のサウンド再生
						ActionAssistComp->PlaySound(ObjectHitSoundObj);
					}
				}

				// ヒットしたアクターが"Enemy"タグを持っていたら
				if (tempActor && tempActor->ActorHasTag("Enemy"))
				{

					// エネミーのdamage処理
					AEnamyBase* Enemy = Cast<AEnamyBase>(HitResult.GetActor());
					AOdaBase* Oda = Cast<AOdaBase>(HitResult.GetActor());

					int ComboNum = Player->GetComboIndex();

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


					// ヒットストップ
					Player->HitStop(0.1f, HitStopTime);

					// コンボのフィニッシュのみカメラを揺らす
					if (ComboNum == 3)
					{
						Player->CameraShake();
					}

					if (EnemyHitSoundObj)
					{
						// 斬撃SE再生
						ActionAssistComp->PlaySound(EnemyHitSoundObj);
					}

					if (Enemy)
					{
						Enemy->ApplyDamage(DamageAmount);

						if (ComboNum == 1)
						{
							if (!Enemy->ActionAssistComp->WallCheck(-100.f))
							{
								Enemy->AddActorLocalOffset(FVector(-100.f, 0.f, 0.f));
							}
						}
						else if (ComboNum == 2)
						{
							if (!Enemy->ActionAssistComp->WallCheck(-100.f))
							{
								Enemy->AddActorLocalOffset(FVector(-100.f, 0.f, 0.f));
							}
						}
						else if (ComboNum == 3)
						{
							if (!Enemy->ActionAssistComp->WallCheck(-250.f))
							{
								Enemy->AddActorLocalOffset(FVector(-250.f, 0.f, 0.f));
							}
						}

						Enemy->ActionAssistComp->HitStop(0.1f, HitStopTime);
					}
					else if (Oda)
					{
						Oda->ApplyDamage(DamageAmount);

						if (Player->GetComboIndex() == 3)
						{
							Oda->BossKnockback();
						}
					}
					if (HitEffect)
					{
						// ヒットエフェクト表示
						ActionAssistComp->SpawnEffect(HitEffect, HitResult.Location);
					}


					break;
				}
			}
		}
	}

}

// 敵の当たり判定
void APlayerSword::EnemyAttack()
{
	AEnamyBase* pEnemy = Cast<AEnamyBase>(pOwner);

	if (pEnemy)
	{
		// 自分とプレイヤーに当たらないようにする
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		CollisionParams.AddIgnoredActor(pEnemy);

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
			// ダメージ量

			float DamageAmount = 5.f;
			// 当たったオブジェクトの数だけ繰り返し
			for (const FHitResult HitResult : HitResults)
			{
				// 当たったキャラクターを格納
				AActor* tempActor = HitResult.GetActor();

				// ヒットしたアクターが"Enemy"タグを持っていたら
				if (tempActor && tempActor->ActorHasTag("Player"))
				{

					// Playerのダメージ処理
					APlayerBase* Player = Cast<APlayerBase>(HitResult.GetActor());

					if (Player)
					{
						Player->TakedDamage(DamageAmount);

						pEnemy->ActionAssistComp->HitStop(0.1f, HitStopTime);

						if (EnemyHitSoundObj)
						{
							// 斬撃SE再生
							ActionAssistComp->PlaySound(EnemyHitSoundObj);
						}
					}

					break;
				}
			}
		}
	}

}
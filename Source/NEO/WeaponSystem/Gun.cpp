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


	// 武器のメッシュのパス
	TCHAR* WeaponAssetPath = TEXT("/Game/0122/Player/Weapon/Gun/Gun");

	// 武器のメッシュ設定
	SetupWeaponMesh(WeaponStaticMesh, WeaponAssetPath);

	// コリジョン設定
	SetupCollisionComponent(WeaponCollision);

	// 武器の種類を刀に設定
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
 * 関数名　　　　：SetCollision()
 * 処理内容　　　：プレイヤーの攻撃の当たり判定
 * 戻り値　　　　：なし
 */
void AGun::SetCollision()
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
void AGun::PlyerAttack()
{
	// プレイヤーのベースクラスにキャスト
	APlayerBase* pPlayer = Cast<APlayerCharacter>(pOwner);

	if (pPlayer)
	{
		// 蹴り攻撃かどうか
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
			// 自分とプレイヤーに当たらないようにする
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);
			CollisionParams.AddIgnoredActor(pPlayer);

			TArray<FHitResult> HitResults;

			float DamageAmount = pPlayer->GetDamageAmount();

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

						if (Enemy)
						{
							Enemy->ApplyDamage(10.f);

							// ノックバック
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

// 敵の当たり判定
void AGun::EnemyAttack()
{
	FVector SpawnBulletLocation = GetActorLocation(); // または他の場所
	FRotator SpawnBulletRotation = pOwner->GetActorRotation(); // または他の回転
	if (BulletClass)
	{
		AActor* SpawnedBullet = GetWorld()->SpawnActor<AActor>(BulletClass, SpawnBulletLocation, SpawnBulletRotation);
	}
}



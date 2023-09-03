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
	TCHAR* WeaponAssetPath = TEXT("/Game/0139/CharacterModel/EnemySwordJoint");

	// 武器のメッシュ設定
	SetupWeaponMesh(WeaponStaticMesh, WeaponAssetPath);

	// コリジョン設定
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
 * 関数名　　　　：SetCollision()
 * 処理内容　　　：プレイヤーの攻撃の当たり判定
 * 戻り値　　　　：なし
 */
void APlayerSword::SetCollision()
{
	// プレイヤーのベースクラスにキャスト
	APlayerBase* pPlayer = Cast<APlayerCharacter>(OwnerInfo.pOwner);

	if (pPlayer)
	{
		// 自身に当たらないようにする
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(pPlayer);

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
						Object->ReceiveDamage(pPlayer->GetDamageAmount());

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
						Enemy->ApplyDamage(pPlayer->GetDamageAmount());

					}
					else if (Oda)
					{
						Oda->ApplyDamage(pPlayer->GetDamageAmount());

						if (pPlayer->GetComboIndex() == 3)
						{
							Oda->BossKnockback();
						}

					}


					// ヒットストップ
					pPlayer->HitStop();

					// コンボのフィニッシュのみカメラを揺らす
					if (pPlayer->GetComboIndex() == 2)
					{
						pPlayer->CameraShake();
					}
				}
			}
		}
	}
}

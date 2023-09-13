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


	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(this, 0);

	SetupOwnerData(pPlayer, "Player", "hand_rSocket");
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
	APlayerBase* pPlayer = Cast<APlayerCharacter>(OwnerInfo.pOwner);

	if (pPlayer)
	{
		FVector SpawnBulletLocation = GetActorLocation();
		FRotator SpawnBulletRotation = pPlayer->GetActorRotation();
		if (BulletClass)
		{
			AActor* SpawnedBullet = GetWorld()->SpawnActor<AActor>(BulletClass, SpawnBulletLocation, SpawnBulletRotation);
		}
	}
}

// 敵の当たり判定
void AGun::EnemyAttack()
{
	FVector SpawnBulletLocation = GetActorLocation(); // または他の場所
	FRotator SpawnBulletRotation = OwnerInfo.pOwner->GetActorRotation(); // または他の回転
	if (BulletClass)
	{
		AActor* SpawnedBullet = GetWorld()->SpawnActor<AActor>(BulletClass, SpawnBulletLocation, SpawnBulletRotation);
	}
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
	: IsHeld(true)
	, IsFalling(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	

	// タグ設定
	Tags.Add("Weapon");
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsFalling)
	{
		BlowsAway();
	}
}

/*
 * 関数名　　　　：SetupWeaponMesh()
 * 処理内容　　　：プレイヤーのメッシュをセットアップ(引数がStaticMeshの場合)
 * 引数１　　　　：UStaticMeshComponent*& MeshComp・・・メッシュコンポーネント
 * 引数２　　　　：TCHAR* WeaponAssetPath ・・・・・・・武器のアセットのパス
 * 引数３　　　　：FName PublicName ・・・・・・・・・・エディタでの公開名
 * 戻り値　　　　：なし
 */
void AWeaponBase::SetupWeaponMesh(UStaticMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName /*= "WeaponMesh"*/)
{
	// 武器のコンポーネントを作成
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(PublicName);

	if (WeaponAssetPath)
	{
		// 武器のアセット設定
		ConstructorHelpers::FObjectFinder< UStaticMesh > weaponMesh(WeaponAssetPath);

		if (weaponMesh.Succeeded())
		{
			MeshComp->SetStaticMesh(weaponMesh.Object);
		}
	}
}


/*
 * 関数名　　　　：AttachToHand()
 * 処理内容　　　：キャラクターにくっつける
 * 引数１　　　　：ACharacter* _owner・・・親になるキャラクター
 * 引数２　　　　：FName _socketName ・・・ソケットの名前
 * 戻り値　　　　：なし
 */
void AWeaponBase::AttachToHand(ACharacter* _owner, FName _socketName,EOwnerType _ownerType)
{
	// 飛んでいるときは取れない
	if (IsFalling) { return; }

	// 持たれている状態にする
	IsHeld = true;

	// オーナーに設定
	OwnerInfo.pOwner = _owner;

	// オーナーの種類を設定
	OwnerType = _ownerType;
	
	// キャラクターにアタッチ
	if (OwnerInfo.pOwner)
	{
		AttachToComponent(OwnerInfo.pOwner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false), _socketName);
	}
}


/*
 * 関数名　　　　：DetachToHand()
 * 処理内容　　　：キャラクターにくっつける
 * 戻り値　　　　：なし
 */
void AWeaponBase::DetachToHand()
{
	// 持たれていない状態にする
	IsHeld = false;

	// キャラクターから外す
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld,false));

	// オーナーがいなくなる
	OwnerInfo.pOwner = nullptr;

	// 吹き飛ばすフラグを建てる
	IsFalling = true;

	// 飛ぶ前の場所設定
	FlyBeforePos = GetActorLocation();

	// Tick処理再開
	PrimaryActorTick.bCanEverTick = true;
}

/*
 * 関数名　　　　：SetupOwnerData()
 * 引数１　　　　：ACharacter* _owner・・・親になるキャラクター
 * 引数２　　　　：FName _ownerTag ・・・・オーナーについているタグ
 * 引数３　　　　：FName _socketName ・・・ソケットの名前
 * 処理内容　　　：オーナーの情報初期化
 * 戻り値　　　　：なし
 */
void AWeaponBase::SetupOwnerData(ACharacter* _owner, FName _ownerTag, FName _socketName)
{
	if (_owner)
	{
		OwnerInfo.pOwner = _owner;
		OwnerInfo.OwnerTag = _ownerTag;
		OwnerInfo.SocketName = _socketName;
	}
}


/*
 * 関数名　　　　：BlowsAway()
 * 処理内容　　　：武器が吹き飛んで地面に刺さる
 * 戻り値　　　　：なし
 */
void AWeaponBase::BlowsAway()
{
	// 現在位置
	FVector NowPos = GetActorLocation();

	// Sinで高さ更新
	float SinValue = 150.f * FMath::Sin(radPerFrame * frames);

	// ジャンプ前の高さから位置更新
	const FVector nextPos(FVector(NowPos.X, NowPos.Y, SinValue + FlyBeforePos.Z));

	SetActorLocation(nextPos);

	// フレーム+1
	frames += 1.f;

	// 回転させる
	FRotator NowRotation = GetActorRotation();

	// 常に回転
	SetActorRotation(FRotator(NowRotation.Pitch, NowRotation.Yaw, NowRotation.Roll + 10.0));

	// 元の位置より低くなったら終了
	if (NowPos.Z < FlyBeforePos.Z)
	{
		// 地面に刺さるように位置と角度を補正
		SetActorLocation(FlyBeforePos);
		SetActorRotation(FRotator(0.f, 0.f, -90.f));

		// フレームリセット
		frames = 0.f;

		// Tick処理中断
		PrimaryActorTick.bCanEverTick = false;

		// 飛んでいる状態のフラグを下ろす
		IsFalling = false;
	}
}



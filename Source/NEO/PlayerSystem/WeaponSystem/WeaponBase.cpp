// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	//pPlayer = UGameplayStatics::GetPlayerCharacter(this, 0);
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// 接触開始時に行う処理
void AWeaponBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// オーバーラップした際に実行したいイベント
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// オーバーラップしたのがプレイヤーの時のみ反応させたい
		// PlayerCharaBP -> Actor -> Tagsに設定したタグ「Player」で、プレイヤーを識別する
		if (OtherActor->ActorHasTag("Player"))
		{
			//// デバッグ確認用
			// UE_LOG(LogTemp,Warning,TEXT("Goal"));



		}
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
 * 関数名　　　　：SetupWeaponMesh()
 * 処理内容　　　：プレイヤーのメッシュをセットアップ(引数がSkeletalMeshの場合)
 * 引数１　　　　：USkeletalMeshComponent*& MeshComp・・・メッシュコンポーネント
 * 引数２　　　　：TCHAR* WeaponAssetPath ・・・・・・・・武器のアセットのパス
 * 引数３　　　　：FName PublicName ・・・・・・・・・・・エディタでの公開名
 * 戻り値　　　　：なし
 */
void AWeaponBase::SetupWeaponMesh(USkeletalMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName /*= "WeaponMesh"*/)
{
	// 武器のコンポーネントを作成
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(PublicName);

	if (WeaponAssetPath)
	{
		// 武器のアセット設定
		ConstructorHelpers::FObjectFinder< USkeletalMesh > weaponMesh(WeaponAssetPath);

		if (weaponMesh.Succeeded())
		{
			MeshComp->SetSkeletalMeshAsset(weaponMesh.Object);
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
void AWeaponBase::AttachToHand(ACharacter* _owner, FName _socketName)
{
	// 持たれている状態にする
	IsHeld = true;

	// キャラクターにアタッチ
	if (_owner)
	{
		AttachToActor(_owner, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative,false), _socketName);
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
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative,false));

	// 吹き飛ばす
	BlowsAway();
}


/*
 * 関数名　　　　：DetachToHand()
 * 引数１　　　　：ACharacter* _owner・・・親になるキャラクター
 * 引数２　　　　：FName _ownerTag ・・・・オーナーについているタグ
 * 引数３　　　　：FName _socketName ・・・ソケットの名前
 * 処理内容　　　：オーナーの情報初期化
 * 戻り値　　　　：なし
 */
void AWeaponBase::InitializeOwnerData(ACharacter* _owner, FName _ownerTag, FName _socketName)
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

}

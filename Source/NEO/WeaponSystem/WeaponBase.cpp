// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NEO/PlayerSystem/ActionAssistComponent.h"
#include "NEO/PlayerSystem/PlayerBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
	: IsHeld(false)
	, IsFalling(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	// タグ設定
	Tags.Add("Weapon");

	// アタックアシストコンポーネント作成
	ActionAssistComp = CreateDefaultSubobject<UActionAssistComponent>(TEXT("AttackAssist"));
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	// プレイヤー取得
	pPlayer = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DistanceCalculationToPlayer();

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
	if (IsFalling || IsHeld) { return; }

	// 持たれている状態にする
	IsHeld = true;

	IsHoldDistance = false;

	// オーナーに設定
	pOwner = _owner;

	// オーナーの種類を設定
	OwnerType = _ownerType;
	
	// キャラクターにアタッチ
	if (pOwner)
	{
		AttachToComponent(pOwner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false), _socketName);
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
	pOwner = nullptr;

	// 吹き飛ばすフラグを建てる
	IsFalling = true;

	// 飛ぶ前の場所設定
	FlyBeforePos = GetActorLocation();
}


/*
 * 関数名　　　　：DistanceCalculationToPlayer()
 * 処理内容　　　：プレイヤーとの距離計算
 * 戻り値　　　　：なし
 */
void AWeaponBase::DistanceCalculationToPlayer()
{
	if (IsHeld || IsFalling) { return; }

	APlayerBase* Player = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Player)
	{
		FVector PlayerPos = Player->GetActorLocation();
		// プレイヤーと距離計算
		float Distance = FVector::Dist(PlayerPos, GetActorLocation());


		if (Distance <= 120.f)
		{
			IsHoldDistance = true;

			if (Cast<APlayerBase>(Player)->GetPickUpWeapon() == nullptr)
			{
				Cast<APlayerBase>(Player)->SetPickUpWeapon(this);
				Cast<APlayerBase>(Player)->SetIsPickUpWeapon(true);
			}

		}
		else
		{
			IsHoldDistance = false;
			if (Cast<APlayerBase>(Player)->GetPickUpWeapon() == this)
			{
				Cast<APlayerBase>(Player)->SetPickUpWeapon(nullptr);
				Cast<APlayerBase>(Player)->SetIsPickUpWeapon(false);
			}
		}
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
		FVector Pos = FVector::ZeroVector;
		
		switch (WeaponType)
		{
		case EWeaponType::WeaponType_Sword:
			break;
		case EWeaponType::WeaponType_Lance:
			Pos.Z = -70.f;
			break;
		case EWeaponType::WeaponType_Gun:
			break;
		case EWeaponType::WeaponType_None:
			break;
		default:
			break;
		}
		// 地面に刺さるように位置と角度を補正
		SetActorLocation(FlyBeforePos + Pos);
		SetActorRotation(DropAngle);

		// フレームリセット
		frames = 0.f;

		// 飛んでいる状態のフラグを下ろす
		IsFalling = false;
	}
}



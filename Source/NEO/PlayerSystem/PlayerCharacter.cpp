// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "PlayerBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/StreamableManager.h"
#include "Engine/World.h"
#include "Engine/AssetManager.h"
#include "Async/Async.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NEO/Enemy/EnamyBase.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// キャラクターコンポーネント取得
	CharacterMovementComp = GetCharacterMovement();
	CharacterMovementComp->MaxWalkSpeed = 500.f;

	// プレイヤーのデータセットアップ
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
	TCHAR* WeaponAssetPath = TEXT("/Game/0122/Player/Weapon/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight");

	// 武器のメッシュ設定
	SetupWeaponMesh(WeaponMesh, WeaponAssetPath, "WeaponMesh");

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
	TCHAR* ComboAnimationAssetPaths[2];

	// アニメーションアセットのパス
	ComboAnimationAssetPaths[0] = TEXT("/Game/0122/Player/Animation/Montage/Combo/SwordCombo");
	ComboAnimationAssetPaths[1] = TEXT("/Game/0122/Player/Animation/Montage/Combo/SwordCombo2");

	for (int i = 0; i < 2; ++i)
	{
		ComboAnimMontages.Add(GetAnimationAsset(ComboAnimationAssetPaths[i]));
	}

	// 被ダメージアニメーションのパス保管用
	TCHAR* DamageAnimationAssetPath = TEXT("/Game/0122/Player/Animation/Montage/Damaged");

	DamageAnimMontage = GetAnimationAsset(DamageAnimationAssetPath);

	TCHAR* DeathAnimationAssetPath = TEXT("/Game/0122/Player/Animation/Montage/Death");

	DeathAnimMontage = GetAnimationAsset(DeathAnimationAssetPath);
}


/*
 * 関数名　　　　：SetCollision()
 * 処理内容　　　：プレイヤーの攻撃の当たり判定(APlayerBaseからオーバーライド)
 * 戻り値　　　　：なし
 */
void APlayerCharacter::SetCollision()
{
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
		for (const FHitResult HitResult : HitResults)
		{
			AActor* tempActor = HitResult.GetActor();
			// ヒットしたアクターが"Enemy"タグを持っていたら
			if (tempActor && tempActor->ActorHasTag("Enemy"))
			{

				// エネミーのdamage処理
				AEnamyBase* Enemy = Cast<AEnamyBase>(HitResult.GetActor());

				if (Enemy)
				{
					// ヒットストップ
					AttackAssistComp->HitStop();
					Enemy->ApplyDamage(GetDamageAmount());
				}
			}
		}
	}
}
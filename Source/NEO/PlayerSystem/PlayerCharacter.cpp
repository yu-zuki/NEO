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
#include "NEO/EnamyBase.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// プレイヤーの設定
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// キャラクターコンポーネント取得
	CharacterMovementComp = GetCharacterMovement();
	CharacterMovementComp->MaxWalkSpeed = 500.f;

	// プレイヤーのデータセット
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

// プレイヤーのデータを初期化
void APlayerCharacter::SetupPlayerData()
{
	Super::SetupPlayerData();

	// 武器のメッシュのパス
	TCHAR* WeaponAssetPath = TEXT("/Game/0122/Player/Weapon/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight");

	WeaponMesh = APlayerBase::SetupWeaponMesh(WeaponMesh, WeaponAssetPath, "WeaponMesh");

	// コリジョン設定
	SetupCollisionComponent(WeaponCollision);

	// アニメーションセットアップ
	TCHAR* AnimationAssetPaths[2];

	// アニメーションアセットのパス
	AnimationAssetPaths[0] = TEXT("/Game/0122/Player/Animation/Montage/Combo/SwordCombo");
	AnimationAssetPaths[1] = TEXT("/Game/0122/Player/Animation/Montage/Combo/SwordCombo2");

	SetupAnimationAsset(AnimationAssetPaths);
}

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
			// ヒットしたアクターが"Enemy"タグを持っていたら
			if (HitResult.GetActor()->ActorHasTag("Enemy"))
			{

				// エネミーのdamage処理
				AEnamyBase* Enemy = Cast<AEnamyBase>(HitResult.GetActor());

				if (Enemy)
				{
					// ヒットストップ
					AttackAssistComp->HitStop();
					Enemy->ApplyDamage(GetDamageAmount(), 0.f);
				}
			}
		}
	}
}
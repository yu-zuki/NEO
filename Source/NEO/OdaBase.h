// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPOdaEnum.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimMontage.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"		//RandRangeを使うために必要なヘッダー
#include "Math/Vector.h"				//Dist(距離)を使うために必要なヘッダー
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Templates/SubclassOf.h"
#include "PlayerSystem/PlayerCharacter.h"

#include "OdaBase.generated.h"


UCLASS()
class NEO_API AOdaBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOdaBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//シーンコンポーネント
	UPROPERTY()
		USceneComponent* Parent;

	//プレイヤーキャラクター
	UPROPERTY()
		AActor* PlayerChara;

	//剣のコリジョン
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BoxComp", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	//キャラクタームーブメント
	UPROPERTY()
		UCharacterMovementComponent* NobunagaMovement;

	UPROPERTY()
		bool SpawnDelay;

	UPROPERTY()
		int SpawnTimer;

	//ダメージ処理-----------------------------------------------------
	UFUNCTION()
	virtual void ApplyDamage(float Damage);

	//ダメージタイプクラス
	UPROPERTY()
		TSubclassOf < class UDamageType > DamageTypeClass;
	//----------------------------------------------------------------

	//時間の取得
	int FlameCounter;

	//列挙型
	ECPPOdaEnum OdaMoveEnum;

	//待機関数
	UPROPERTY()
	bool SwitchStayMove;

	//int型のカウンター
	int WaitTime;

	//ボスとプレイヤーとの距離----------------------------
	//X軸
	//ボス
	FVector BossPosX;
	//プレイヤー
	FVector PlayerPosX;
	//Y軸
	//ボス
	FVector BossPosY;
	//プレイヤー
	FVector PlayerPosY;
	//---------------------------------------------------


	//ボスのステートでの処理----------------
	void OdaStay1(int Timer);

	void OdaBack1(int Timer);

	void OdaAttack1(int Timer);

	void OdaAttack2(int Timer);

	void OdaUlt(int Timer);
	//------------------------------------

	//速度----------------------------------
	//通常の移動速度
	UPROPERTY(EditAnywhere)
		float OdaSpeed;

	//急な速度
	UPROPERTY(EditAnywhere)
		float FastOdaSpeed;
	//--------------------------------------

	//近接攻撃が早すぎてプレイヤーがよけれないので少し遅延させる
	UPROPERTY()
	int Attack1Delay;

	void OdaMove1(int DeltaTime, int StopTimer);

	//行動変更時間設定
	UPROPERTY(EditAnywhere)
		int ChangeFlontTimer;

	//モーションを一回だけ流すための変数
	bool isMotionPlaying;

	//衝撃波の出現タイミングの調整
	UPROPERTY()
		bool isShockWaveSpawnTiming;

	//衝撃波の出現タイミングの調整するための関数
	UFUNCTION()
		void ShockWaveSpawnFlagChange();

	//衝撃波を出現させる為の変数
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor>ShockWaveSpawn;

	//必殺技の出現タイミングの調整
	UPROPERTY()
		bool isUltSpawnTiming;

	//必殺技を打つタイミングの調整
	UPROPERTY()
		bool isUltShotTiming;

	//必殺技の出現タイミングの調整するための関数
	UFUNCTION()
		void UltSpawnFlagChange();

	//必殺技を出現させる為の変数
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor>UltSpawn;

	//必殺技は一回だけ出したいのでフラグで管理
	UPROPERTY()
		bool isUltShot;

	UPROPERTY()
		int UltTimer;

	//攻撃のフラグ
	UPROPERTY(BlueprintReadOnly)
		bool IsAttackNow;

	//攻撃のフラグを切り替える為の関数
	UFUNCTION()
		void AttackFlagChange();

	//攻撃された回数が一定数超えたら近接攻撃をぱなす為の変数
	UPROPERTY()
		bool isNotAttackNow;
	//一定数の計測
	UPROPERTY()
		int NotAttackCount;



	//HPが連続で減らないようにロック
	UPROPERTY()
		bool isHPLock;

	//HPが連続で減らないようにロックするための関数
	UFUNCTION()
		void HPLock();

	//プレイヤーの方向に向ける処理
	UFUNCTION()
		void ToPlayerRotate();

	//ノックバックするモーション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossKnockMontage;		//メモ：変数を作る際AnimMontage_は必須らしい

	//攻撃する仮モーション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossAttack1;

	//衝撃波攻撃する仮モーション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossAttack2;

	//衝撃波攻撃する仮モーション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossUltimate;

	//前方移動
	UFUNCTION()
		void BossMove(float Speed , FVector MoveSize);

	//後方移動
	UFUNCTION()
		void BackMove(float Speed);

	//ダメージ値
	UPROPERTY(EditAnywhere, Category = "Damage")
		float SwordDamage;

	UFUNCTION()
		void CheckOverlap();

	UFUNCTION()
		void EnemyOnOverlap(FHitResult& _HitResult);

	UPROPERTY()
		bool bIsAttacked;

	UFUNCTION()
		void toPlayerAttacked();

//////////////////////////////////////////////////////////////////////////
//死亡処理
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area")
		bool IsAreaEnemy = false;

	void Death();

//////////////////////////////////////////////////////////////////////////
//UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		class UEnemyBase_WidgetComponent* EnemyWidget;

	//体力
	UPROPERTY(EditAnywhere)
		float Health;

	UPROPERTY(EditAnywhere)
		float MaxHealth;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPOdaEnum.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"			//キャラクタームーブメントを使うために必要なヘッダー
#include "Kismet/GameplayStatics.h"								//getplayerpawn等を使うために必要なヘッダー
#include "Kismet/KismetSystemLibrary.h"							//printStringを使うために必要なヘッダー
#include "Animation/AnimMontage.h"								//アニメーションを流すために必要なヘッダー
#include "Math/UnrealMathUtility.h"								//RandRangeを使うために必要なヘッダー
#include "Math/Vector.h"										//Dist(距離)を使うために必要なヘッダー
#include "Components/BoxComponent.h"							//ボックスコンポーネントを使うため
#include "Templates/SubclassOf.h"								//TSubclassOfを使うために必要なヘッダー
#include "PlayerSystem/PlayerCharacter.h"						//プレイヤーキャラを取得するために必要なヘッダー
#include "NiagaraComponent.h"									//ナイアガラエフェクトを呼ぶために必要なヘッダー
#include "NiagaraFunctionLibrary.h"								//ナイアガラエフェクトを呼ぶために必要なヘッダー
#include "NEO/PlayerSystem/ActionAssistComponent.h"

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

	//コンポーネント-----------------------------------------------------------------------------------------------------
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
		class UActionAssistComponent* ActionAssistComp;
	//--------------------------------------------------------------------------------------------------------------------

	//スポーン時処理--------------------------------------------------------------------------
	//スポーン中の止まるときか
	UPROPERTY()
		bool SpawnDelay;
	
	//スポーン中止まる時間
	UPROPERTY()
		int SpawnTimer;
	//----------------------------------------------------------------------------------------

	//ダメージ処理-----------------------------------------------------
	UFUNCTION()
	virtual void ApplyDamage(float Damage);

	//ダメージタイプクラス
	UPROPERTY()
		TSubclassOf < class UDamageType > DamageTypeClass;
	//----------------------------------------------------------------

	//カウンター------------------------------------------------------------
	//時間の取得
	int FlameCounter;

	//int型のカウンター
	int WaitTime;
	//---------------------------------------------------------------------
	
	//列挙型
	ECPPOdaEnum OdaMoveEnum;

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

	void OdaAttack1(int Timer);

	void OdaAttack2(int Timer);

	void OdaUlt(int Timer);
	//------------------------------------

	//速度----------------------------------
	//通常の移動速度
	UPROPERTY(EditAnywhere)
		float OdaSpeed;
	//--------------------------------------

	//近接攻撃が早すぎてプレイヤーがよけれないので少し遅延させる
	UPROPERTY()
	int Attack1Delay;

	//ボスの動きの処理
	void OdaMove1(int DeltaTime, int StopTimer);

	UPROPERTY()
		int RandomNum;

	//もう一回衝撃波を出すための変数
	UPROPERTY()
		bool OneMoreShockWave;

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

	//必殺技---------------------------------------------------------------------------
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

	//必殺技をもう一回打つためのカウンター
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

	//ヒットエフェクト変数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UNiagaraSystem* HitParticles;

	//HPが連続で減らないようにロック
	UPROPERTY()
		bool isHPLock;

	//HPが連続で減らないようにロックするための関数
	UFUNCTION()
		void HPLock();

	//プレイヤーの方向に向ける処理
	UFUNCTION()
		void ToPlayerRotate();

	//のけぞるモーション
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

	//ふっとぶモーション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossBlowAway;


	//ノックバックしてふっとぶ関数
	UFUNCTION()
		void BossKnockback();

	//前方移動
	UFUNCTION()
		void BossMove(float Speed , FVector MoveSize);

	//ダメージ値
	UPROPERTY(EditAnywhere, Category = "Damage")
		float SwordDamage;

	//ボックスコンポーネントのオーバーラップ処理
	UFUNCTION()
		void CheckOverlap();

	//プレイヤーに当たったら
	UFUNCTION()
		void PlayerOnOverlap(FHitResult& _HitResult);

	//プレイヤーに対してのHPロック
	UPROPERTY()
		bool bIsAttacked;

	//プレイヤーに対してのHPロックするための関数
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

protected:
	AActor* GetPlayer();
};

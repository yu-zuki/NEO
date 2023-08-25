// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NEO/GameSystem/InputCharacter.h"
#include <unordered_map>
#include <type_traits>
#include "ActionAssistComponent.h"
#include <Runtime/Engine/Classes/Components/CapsuleComponent.h>

#include "PlayerBase.generated.h"

// 前方宣言
class UBoxComponent;
class USphereComponent;
class UCapsuleComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class USplineComponent;


//-----------------inputAction------------------------------------------------------------
USTRUCT(BlueprintType)
struct FMainAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputAction* ComboAction1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputAction* ComboAction2;
};
//----------------------------------------------------------------------------------------

//-----------------プレイヤーのステータス-------------------------------------------------
USTRUCT(BlueprintType)
struct FPlayerStatus
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int RemainingLife;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float ComboDamageFactor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float JumpHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float NowMoveSpeed;
};
//----------------------------------------------------------------------------------------

//-----------------アニメーション保管用---------------------------------------------------
USTRUCT(BlueprintType)
struct FPlayerAnimation
{
	GENERATED_BODY()

	// コンボ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		TArray<UAnimMontage*> Combo;

	// 空中にいるときの攻撃
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* AirAttack;

	// 被ダメージ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* TakeDamage;

	// 被ダメージ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* KnockBack;

	// 死亡時
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* Death;
};
//----------------------------------------------------------------------------------------

UCLASS()
class NEO_API APlayerBase : public AInputCharacter
{
	GENERATED_BODY()

public:

	// コンストラクタ
	APlayerBase();

protected:

	//----------------入力で呼び出される関数-----------------------------
	// 移動
	virtual void Move(const FInputActionValue& _value);

	// ダッシュ切り替え
	void Run();

	// ジャンプ
	virtual void JumpStart();
	virtual void Jump();

	// 攻撃
	virtual void Attack(int _attackNum = 0);

	// 一つ目のコンボ
	virtual void Combo1();

	// 二つ目のコンボ
	virtual void Combo2();
	//-------------------------------------------------------------------


public:

	//-----------------他クラスで呼び出し可--------------------------------------------------------------------------------------------
	// コンボ継続
	void ContinuationCombo();

	// コンボリセット
	void ResetCombo();

	int GetComboIndex()const { return ComboIndex; }

	// ダメージを与える処理
	virtual void SetCollision() { return; }
	
	// 操作可・不可を切り替える処理
	void SetControl(bool _isControl) { IsControl = _isControl; }

	// 死亡時のアニメーションの再生を遅くする
	void SlowDownDeathAnimationRate();

	// ダメージ量を返す関数
	float GetDamageAmount()const { return PlayerStatus.DamageAmount * (((float)ComboIndex + 1.f) * PlayerStatus.ComboDamageFactor); }

	// 現在のHPを返す
	UFUNCTION(BlueprintCallable, Category = "GetStatus")
		float GetHP()const { return PlayerStatus.HP; }

	// 現在の移動量を返す
	float GetDistanceAdvanced() { return DistanceAdvanced * deltaTime;}

	// ダメージを受ける処理
	UFUNCTION(BlueprintCallable, Category = "Action")
		void TakedDamage(float _damage, bool _isLastAttack = false);

	// プレイヤーが地面にいるか
	bool IsPlayerGrounded()const;
	//---------------------------------------------------------------------------------------------------------------------------------------------------


	//-------------------------スプライン------------------------------
public:
	FRotator SplineYawRotation;

private:

	// キャラクターの回転
	void RotateCharacter(float _nowInput_X);

	// キャラクターの移動量取得
	void AmountOfMovement(FVector _nowPos);

	// 死亡処理呼び出し
	void CallGameModeFunc_DestroyPlayer();

	// スプライン検索
	AActor* GetSplineActor(const FName _tag);

	// アニメーション再生
	void PlayAnimation(UAnimMontage* _toPlayAnimMontage, FName _startSectionName = "None", float _playRate = 1.f);

protected:

	// ゲーム開始時に呼び出される処理
	virtual void BeginPlay() override;

	// 毎フレーム呼び出される処理
	virtual void Tick(float DeltaTime) override;

	// 入力のセットアップ
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//------------------プレイヤーのセットアップ---------------------------------------------------------------------------------------------------------
	// プレイヤーのデータを初期化
	virtual void SetupPlayerData();

	// プレイヤーのステータス初期化
	void SetupPlayerStatus(float _hp = 100.f, int _remainingLife = 3.f, float _damageAmount = 10.f,
							float _jumpHeight = 150.f, float _comboDamageFactor = 1.f, float _walkSpeed = 10.f, float _runSpeed = 30.f);

	// ボタンの設定
	void SetupMainActionMapping();

	// 引数によってスタティックメッシュかスケルタルメッシュのセットアップ
	void SetupWeaponMesh(UStaticMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName = "WeaponMesh");
	void SetupWeaponMesh(USkeletalMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName = "WeaponMesh");

	// 指定したパスのアニメーションアセットを返す
	UAnimMontage* GetAnimationAsset(TCHAR* _animAssetPath);


	// ---------コリジョンコンポーネント作成テンプレート
	/*
	 * 関数名　　　　：SetupCollisionComponent()
	 * 処理内容　　　：武器のコリジョンセットアップ
	 * 引数１　　　　：T* CollisionComp・・・・・・・・・コリジョンコンポーネント(「T」は UBoxComponent,USphereComponent,UCapsuleComponent のいずれか)
	 * 引数２　　　　：FName PublicName・・・・・・・・・エディタでの公開名
	 * 戻り値　　　　：なし
	 */
	template<class T>
	void SetupCollisionComponent(T*& CollisionComp, FName PublicName = "CollisionComp")
	{
		static_assert(std::is_same<T, UBoxComponent>::value || std::is_same<T, USphereComponent>::value || std::is_same<T, UCapsuleComponent>::value,
			"「T」は UBoxComponent,USphereComponent,UCapsuleComponent のいずれか ");

		// 対応するオブジェクト生成
		CollisionComp = CreateDefaultSubobject<T>(PublicName);

		if (CollisionComp)
		{
			// 武器のメッシュに追従
			CollisionComp->SetupAttachment(GetMesh(), "hand_rSocket");
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------


	//-----------------プレイヤー管理用変数------------------------------------------------------------------
private:

	// プレイやーのアクション管理用
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FMainAction MainActionMapping;

protected:

	// プレイヤーのステータス管理用
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FPlayerStatus PlayerStatus;

	// プレイヤーのアニメーション管理用
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FPlayerAnimation PlayerAnimation;

	//-------------------------------------------------------------------------------------------------------------


	//-----------------コンポーネント変数--------------------------------------------------------------------------
	// 武器のメッシュ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Mesh", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* WeaponMesh;

	// 攻撃のアシスト用コンポーネント
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action Assist", meta = (AllowPrivateAccess = "true"))
		class UActionAssistComponent* ActionAssistComp;

	// 被ダメージ時のエフェクト
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
		class UNiagaraSystem* HitEffect;

	// カメラの揺れ方
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShakeBase> ShakePattern;

	// キャラクターの動き
	UCharacterMovementComponent* CharacterMovementComp;
	//-------------------------------------------------------------------------------------------------------------

protected:

	// 何秒間ヒットストップを起こすか
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Action Assist")
		float HitStopTime = 0.2f;

	// 死亡アニメーションで倒れてからの再生スピード(1で通常スピード)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),Category = "GameOver")
		float DeadAnimRate = 0.01f;							

	// 死んでからゲームオーバーまでの時間(秒)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float DeadToGameOverTime = 3.f;					

	// 移動した距離
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float DistanceAdvanced;

private:

	// 入力可能かどうか
	bool IsControl;					

	// ダッシュ中かどうか
	bool IsRunning;		

	// ジャンプ中かどうか
	bool IsJumping;

	// フレームカウント用
	float frames;	

	// ジャンプの計算
	const float radPerFrame = 3.14f / 30.f;

	// ジャンプ前の高さ
	float JumpBeforePos_Z;	

	// 前の位置
	FVector BeforePos;

	// 攻撃中かどうか
	bool IsAttacking;						

	// コンボ継続できるか
	bool CanCombo;							

	// 何段目のコンボか
	int ComboIndex;							

	// コンボの段数(First,Second,Third・・・)
	TArray<FName> ComboStartSectionNames;	

	// ハンドル
	FTimerHandle TimerHandle_DeathToGameOver;		

	// プレイヤーが通るスプライン
	class APlayerSpline* SplineActor;		

	// ゲームモード保存
	class ATGS_GameMode* pGameMode;

	// デルタタイム保存用
	float deltaTime;

//////////////////////////////////////////////////////////////////////////
///UI
public:
	//プレイヤーのHPの％を返す
	UFUNCTION(BlueprintCallable, Category = "UI")
		float GetPlayerHPPercent()const { return PlayerStatus.HP / PlayerStatus.MaxHP; }

	class UTGS_GameInstance* GetGameInstance();

};
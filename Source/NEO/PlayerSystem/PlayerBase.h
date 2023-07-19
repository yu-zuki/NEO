// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NEO/GameSystem/InputCharacter.h"
#include <unordered_map>
#include <type_traits>
#include "AttackAssistComponent.h"
#include "PlayerBase.generated.h"

// 前方宣言
class UBoxComponent;
class USphereComponent;
class UCapsuleComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;


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

//-----------------PlayerStatus------------------------------------------------------------
USTRUCT(BlueprintType)
struct FPlayerStatus
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float ComboDamageFactor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float JumpHeight;
};
//----------------------------------------------------------------------------------------

UCLASS()
class NEO_API APlayerBase : public AInputCharacter
{
	GENERATED_BODY()

	// プレイヤーのステート
	enum Player_State
	{
		State_Idle = 0,
		State_Jump,
		State_TakeDamage,
		State_Death
	};


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FMainAction MainActionMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FPlayerStatus PlayerStatus;

public:
	// Sets default values for this character's properties
	APlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	//----------------入力で呼び出される関数-----------------------------
	// 移動
	void Move(const FInputActionValue& _value);

	// ダッシュ切り替え
	void Run();

	// ジャンプ
	void JumpStart();
	void Jump();

	// プレイヤーが地面にいるか
	bool IsPlayerGrounded()const;

	// 攻撃
	virtual void Attack(int _attackNum = 0);

	// 一つ目のコンボ
	virtual void Combo1();

	// 二つ目のコンボ
	virtual void Combo2();
	//-------------------------------------------------------------------

protected:

	//-----------------AnimBPで呼び出し(攻撃や被ダメージ処理)--------------------------------------------------------------------------------------------
	// コンボ継続
	UFUNCTION(BlueprintCallable,Category = "ComboAction")
		void ContinuationCombo();

	// コンボリセット
	UFUNCTION(BlueprintCallable, Category = "ComboAction")
		void ResetCombo();

	// ダメージを与える処理
	UFUNCTION(BlueprintCallable, Category = "ComboAction")
		virtual void SetCollision();

	// ダメージ量を返す関数
	UFUNCTION(BlueprintCallable,Category = "GetStatus")
		float GetDamageAmount()const { return PlayerStatus.DamageAmount * (((float)ComboIndex + 1.f) * PlayerStatus.ComboDamageFactor); }

	// ダメージを受ける処理
	UFUNCTION(BlueprintCallable, Category = "GetStatus")
		void TakedDamage(float _damage);

	UFUNCTION(BlueprintCallable, Category = "GetStatus")
		float GetHP()const { return PlayerStatus.HP; }
	//---------------------------------------------------------------------------------------------------------------------------------------------------


	// アニメーション再生
	void PlayAnimation(UAnimMontage* _toPlayAnimMontage, FName _startSectionName = "None", float _playRate = 1.f);

private:

	// キャラクターの回転
	void RotateCharacter(float _nowInput_Y);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//------------------プレイヤーのセットアップ---------------------------------------------------------------------------------------------------------
	// プレイヤーのデータを初期化
	virtual void SetupPlayerData();

	// プレイヤーのステータスパラメータ初期化
	void SetupPlayerStatus(float _hp = 100.f, float _damageAmount = 10.f, float _jumpHeight = 150.f, float _comboDamageFactor = 1.f);

	// ボタンの設定
	void SetupMainActionMapping();

	// アニメーションの設定
	void SetupAnimationAsset(TCHAR* AnimAssetPath[2]);

	// 引数によってスタティックメッシュかスケルタルメッシュのセットアップ
	void SetupWeaponMesh(UStaticMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName = "WeaponMesh");
	void SetupWeaponMesh(USkeletalMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName = "WeaponMesh");

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


	//-----------------コンポーネント変数--------------------------------------------------------------------------
	// 武器のメッシュ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* WeaponMesh;

	// 攻撃のアシスト用コンポーネント
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackAssist, meta = (AllowPrivateAccess = "true"))
		class UAttackAssistComponent* AttackAssistComp;

	// キャラクターの動き
	UCharacterMovementComponent* CharacterMovementComp;
	//-------------------------------------------------------------------------------------------------------------


	// アニメーション保管用
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
		TArray<UAnimMontage*> ComboAnimMontages;

protected:

	bool IsControl;					// 入力可能かどうか

	bool IsRunning;					// ダッシュ中のフラグ

	float frames;					// フレーム		

	const float radPerFrame = 3.14f / 30.f;

	float JumpBeforePos_Z;					// ジャンプ前の高さ

	Player_State PlayerState;				// プレイヤーのステート

	bool IsAttacking;						// 攻撃中のフラグ

	bool CanCombo;							// コンボ継続できるか

	int ComboIndex;							// 何段目のコンボか

	TArray<FName> ComboStartSectionNames;	// コンボの段数(First,Second,Third・・・)
};
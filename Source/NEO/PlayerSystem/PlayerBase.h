// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NEO/GameSystem/InputCharacter.h"
#include <unordered_map>
#include <type_traits>
#include "PlayerBase.generated.h"

// 前方宣言
class UBoxComponent;
class USphereComponent;
class UCapsuleComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;


// inputAction
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

// inputAction
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

UCLASS()
class NEO_API APlayerBase : public AInputCharacter
{
	GENERATED_BODY()

	// プレイヤー
	enum Player_State
	{
		State_Idle = 0,
		State_Jump,
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
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	// ダッシュ切り替え
	void Run();

	// ジャンプ
	void JumpStart();
	void Jump();

	// プレイヤーが地面にいるか
	bool IsPlayerGrounded()const;

	// 攻撃
	virtual void Attack(int AttackNum = 0);

	// 一つ目のコンボ
	virtual void Combo1();

	// 二つ目のコンボ
	virtual void Combo2();

protected:

	// キャラクターの回転
	void RotateCharacter(float nowInput_Y);

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

	// アニメーション再生
	void PlayAnimation(UAnimMontage* _toPlayAnimMontage, FName _startSectionName = "None", float _playRate = 1.f);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// プレイヤーのデータを初期化
	virtual void SetupPlayerData();

	// プレイヤーのステータスパラメータ初期化
	void SetupPlayerStatus(float _hp = 100.f, float _damageAmount = 10.f, float _jumpHeight = 150.f, float _comboDamageFactor = 1.f);

	// ボタンの設定
	void SetupMainActionMapping();

	// アニメーションの設定
	void SetupAnimationAsset(TCHAR* AnimAssetPath[2]);

	/**
	 * @brief 引数がスタティックメッシュの場合の関数
	 *
	 * @param MeshComp - 武器のコンポーネント
	 * @param WeaponAssetPath - 武器のメッシュアセットのパス
	 * @param PublicName - 武器の公開名
	 * @return UStaticMeshComponent* - 武器のコンポーネント
	*/
	UStaticMeshComponent* SetupWeaponStaticMesh(class UStaticMeshComponent* MeshComp, TCHAR* WeaponAssetPath, FName PublicName)
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

			// 体のメッシュに追従
			MeshComp->SetupAttachment(GetMesh(), "hand_rSocket");
		}

		return MeshComp;
	}

	/**
	* @brief 引数がスケリタルメッシュの場合の関数
	*
	 * @param MeshComp  - 武器のコンポーネント
	 * @param WeaponAssetPath - 武器のメッシュアセットのパス
	 * @param PublicName - 武器の公開名
	 * @return USkeletalMeshComponent* - 武器のコンポーネント
	*/
	USkeletalMeshComponent* SetupWeaponSkeletalMesh(USkeletalMeshComponent* MeshComp, TCHAR* WeaponAssetPath, FName PublicName)
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

			// 体のメッシュに追従
			MeshComp->SetupAttachment(GetMesh(), "hand_rSocket");
		}

		return MeshComp;
	}

	//2023/7/14 16:20 董：Error解決↓ (Castを使用することで　SetupWeaponStaticMesh　の　template<class U>　を消すことで解決できました)
	template<class U>
	U* SetupWeaponMesh(U* MeshComp,TCHAR* WeaponAssetPath, FName PublicName = "WeaponMesh")
	{
		// 判定
		if (std::is_same<U, UStaticMeshComponent>::value)
		{
			APlayerBase::SetupWeaponStaticMesh(Cast<UStaticMeshComponent>(MeshComp), WeaponAssetPath, PublicName);
		}
		else if  (std::is_same<U, USkeletalMeshComponent>::value)
		{
			APlayerBase::SetupWeaponSkeletalMesh(Cast<USkeletalMeshComponent>(MeshComp), WeaponAssetPath, PublicName);
		}

		return MeshComp;
	}

	// コリジョンコンポーネント作成テンプレート
	template<class T>
	T* SetupCollisionComponent(T* CollisionComp, FName PublicName = "CollisionComp")
	{
		static_assert(std::is_same<T, UBoxComponent>::value || std::is_same<T, USphereComponent>::value || std::is_same<T, UCapsuleComponent>::value,
			"「T」は UBoxComponent,USphereComponent,UCapsuleComponent のいずれか ");

		// 対応するオブジェクト生成
		CollisionComp = CreateDefaultSubobject<T>(PublicName);

		// 武器のメッシュに追従
		CollisionComp->SetupAttachment(WeaponMesh);

		return CollisionComp;
	}

	// 武器のメッシュ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* WeaponMesh;

	// アニメーション保管用
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
		TArray<UAnimMontage*> ComboAnimMontages;

	// キャラクターの動き
	UCharacterMovementComponent* CharacterMovementComp;

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
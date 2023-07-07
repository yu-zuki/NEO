// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameSystem\InputCharacter.h"
#include "PlayerBase.generated.h"

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

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* ComboAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* ComboAction2;

public:
	// Sets default values for this character's properties
	APlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	void Attack(int AttackNum = 0);

	// 一つ目のコンボ
	void Combo1();

	// 二つ目のコンボ
	void Combo2();

	// キャラクターの回転
	void RotateCharacter(float nowInput_Y);

	// コンボ継続
	UFUNCTION(BlueprintCallable)
		void ContinuationCombo();

	// コンボリセット
	UFUNCTION(BlueprintCallable)
		void ResetCombo();

	// ダメージを与える処理
	UFUNCTION(BlueprintCallable)
		virtual void SetCollision();

	// ダメージ量を返す関数
	UFUNCTION(BlueprintCallable)
		float GetDamageAmount()const { return DamageAmount * ((float)ComboIndex + 1.f); }

	// ダメージを受ける処理
	UFUNCTION(BlueprintCallable)
		void TakedDamage(float _damage);

	UFUNCTION(BlueprintCallable)
		float GetHP()const { return HP; }

	// アニメーション再生
	void PlayAnimation(UAnimMontage* ToPlayAnimMontage, FName StartSectionName = "None");

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ボタンの設定
	void SetupDefoultMappingContext();

	// アニメーションの設定
	virtual void SetupAnimationAsset(TCHAR* AnimAssetPath[2]);

	// 武器のメッシュの設定
	virtual void SetupWeapon(TCHAR* WeaponAssetPath,FName PublicName = "Weapon");

	virtual void SetupCollision();

	// 武器のメッシュ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* WeaponMesh;

	//武器の当たり判定
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* WeaponCollision;

	// アニメーション保管用
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
		TArray<UAnimMontage*> ComboAnimMontages;

	// キャラクターの動き
	UCharacterMovementComponent* CharacterMovementComp;

protected:

	bool IsControl;					// 入力可能かどうか

	bool IsRunning;					// ダッシュ中のフラグ

	float frames;					// フレーム		

	UPROPERTY(EditAnywhere)
		float height;				// ジャンプの高さ

	const float radPerFrame = 3.14f / 30.f;

	float JumpBeforePos_Z;			// ジャンプ前の高さ

	Player_State PlayerState;		// プレイヤーのステート

	bool IsAttacking;				// 攻撃中のフラグ

	bool CanCombo;					// コンボ継続できるか

	int ComboIndex;					// 何段目のコンボか

	TArray<FName> ComboCntNames;	// コンボの段数(First,Second,Third・・・)

	UPROPERTY(EditAnywhere, Category = Damage)
		float DamageAmount;			// 与ダメージ量

	float HP;						// HP
};

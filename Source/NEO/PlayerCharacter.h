// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class USkeletalMeshComponent;
class UCharacterMovementComponent;

UCLASS()
class NEO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	// 属性
	enum Attribute_State
	{
		State_Fire = 0,
		State_Ice,
		State_Wind
	};

private:

	// コンボ
	enum Attack_State
	{
		State_Combo1 = 0,
		State_Combo2,
		State_Ult
	};

	// プレイヤー
	enum Player_State
	{
		State_Idle = 0,
		State_Jump
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
		UInputAction* SwitchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* ComboAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* ComboAction2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* UltAction;


public:
	// Sets default values for this character's properties
	APlayerCharacter();

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
	void Attack();

	// 一つ目のコンボ
	void Combo1();

	// 2つ目のコンボ
	void Combo2();

	// コンボリセット
	void ResetCombo();

	// 必殺技
	void UltimateAttack();

	// ダメージを受ける処理
	void TakedDamage();

	// 属性切り替え
	void SwitchAttribute();

	// アニメーション再生
	void PlayAnimation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ボタンの設定
	void SetupDefoultMappingContext();

	// アニメーションの設定
	void SetupAnimationAsset();

	UFUNCTION(BlueprintCallable)
		int GetAttribute()const { return AttrState; }


	// 剣のメッシュ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* SwordMesh;

	// アニメーション保管用
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
		TArray<UAnimMontage*> ComboAnimMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
		UAnimMontage* UltAnimMontage;

	// キャラクターの動き
	UCharacterMovementComponent* CharacterMovementComp;


private:

	bool IsControl;					// 入力可能かどうか

	bool IsRunning;					// ダッシュ中のフラグ

	float frames;					// フレーム		

	UPROPERTY(EditAnywhere)
		float height = 150;			// ジャンプの高さ

	const float radPerFrame = 3.14f / 30.f;

	float JumpBeforePos_Z;			// ジャンプ前の高さ

	Player_State PlayerState;		// プレイヤーのステート

	Attribute_State AttrState;		// 属性のステート

	bool IsAttacking;				// 攻撃中のフラグ

	Attack_State AttackState;		// 攻撃のステート

	bool CanCombo;					// コンボ継続できるか

	int ComboIndex;					// 何段目のコンボか

	TArray<FName> ComboCntNames;	// 何段目のコンボの名前(First,Second,Third・・・)

	UPROPERTY(EditAnywhere, Category = Damage)
	float DamageAmount;				// ダメージ量
};

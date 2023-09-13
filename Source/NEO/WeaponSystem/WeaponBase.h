// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


class ACharacter;

//-----------------所持者の種類-------------------------------------------------------------
UENUM(BlueprintType)
enum class EOwnerType :uint8
{
	OwnerType_Player   UMETA(DisplayName = "PlayerType"),
	OwnerType_Enemy    UMETA(DisplayName = "EnemyType"),
	OwnerType_Boss     UMETA(DisplayName = "BossType"),
};
//----------------------------------------------------------------------------------------

//-----------------武器の種類-------------------------------------------------------------
UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	WeaponType_Sword   UMETA(DisplayName = "Sword"),
	WeaponType_Lance   UMETA(DisplayName = "Lance"),
	WeaponType_Gun     UMETA(DisplayName = "Gun"),
	WeaponType_None	   UMETA(DisplayName = "None")
};
//----------------------------------------------------------------------------------------


UCLASS()
class NEO_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// StaticMeshのセットアップ
	void SetupWeaponMesh(UStaticMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName = "WeaponMesh");

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
			CollisionComp->SetupAttachment(WeaponStaticMesh, "hand_rSocket");
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------

protected:

	// 外れた時吹っ飛ぶ
	void BlowsAway();

	// プレイヤーの攻撃記述用
	virtual void PlayerAttack() { return; }

	// 敵の攻撃記述用
	virtual void EnemyAttack() { return; }

	// ボスの攻撃記述用
	virtual void BossAttack() { return; }

public:

	// プレイヤーの手に付ける
	void AttachToHand(ACharacter* _owner, FName _socketName, EOwnerType _ownerType);

	// プレイヤーの手から外れる
	void DetachToHand();

	// ダメージを与える処理(オーバーライド用)
	virtual void SetCollision() { return; }

	// 持たれている状態か
	UFUNCTION(BlueprintCallable, Category = "State")
		bool GetIsHeld()const { return IsHeld; }

	// 飛んでいる状態か
	UFUNCTION(BlueprintCallable, Category = "State")
		bool GetIsFalling()const { return IsFalling; }

	// オーナーの種類判別用
	UFUNCTION(BlueprintCallable, Category = "State")
		EOwnerType GetOwnerType()const { return OwnerType; }

	// 武器の種類判別用
	EWeaponType GetWeaponType()const { return WeaponType; }

	// 武器のメッシュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponMesh")
		class UStaticMeshComponent* WeaponStaticMesh;

	// 武器のコリジョン
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponCollision")
		class UCapsuleComponent* WeaponCollision;

	// オーナーを判別するEnum
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OwnerType")
		EOwnerType OwnerType;

	// 武器を判別するEnum
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponType")
		EWeaponType WeaponType;

protected:

		// アクションアシストコンポーネント
		class UActionAssistComponent* ActionAssistComp;

		// オーナーになるキャラクターの情報
		class ACharacter* pOwner = nullptr;


private:

	// プレイヤーに持たれているかのフラグ
	bool IsHeld;

	// 飛んでいるかどうか
	bool IsFalling;

	// フレームカウント用
	float frames;

	// ジャンプの計算
	const float radPerFrame = 3.14f / 30.f;

	// 飛ぶ前の位置
	FVector FlyBeforePos;

	// 被ダメージ時のエフェクト
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
		class UNiagaraSystem* AuraEffect;
};

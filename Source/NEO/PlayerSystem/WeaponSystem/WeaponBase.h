// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


class ACharacter;

//-----------------OwnerInformation-------------------------------------------------------
USTRUCT(BlueprintType)
struct FOwnerInfo
{
	GENERATED_BODY()

public:

	// オーナーになるキャラクターの情報
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		ACharacter* pOwner;

	// オーナーのタグ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FName OwnerTag;

	// アタッチするソケットの名前
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FName SocketName;
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

	// 接触開始時に行う処理
	UFUNCTION()
		virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

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
			CollisionComp->SetupAttachment(WeaponStaticMesh, "hand_rSocket");
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------

public:	

	// プレイヤーの手に付ける
	void AttachToHand(ACharacter* ParentCharacter, FName SocketName);

	// プレイヤーの手から外れる
	void DetachToHand();


protected:

	// オーナーのデータ初期化
	void InitializeOwnerData(ACharacter* _owner,FName _ownerTag,FName _socketName);

	// 外れた時吹っ飛ぶ
	void BlowsAway();

	// ダメージを与える処理(オーバーライド用)
	virtual void SetCollision() { return; }

private:

	FOwnerInfo OwnerInfo;

	// プレイヤーに持たれているかのフラグ
	bool IsHeld;
};

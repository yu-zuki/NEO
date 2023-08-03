// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterCamera.generated.h"

UCLASS()
class NEO_API ACharacterCamera : public AActor
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Follow_Camera;

	// プレイヤーの情報
	ACharacter* PlayerInfo;

	// 初期位置
	FVector StartPos;

	// プレイヤーから注視点の位置までの距離
	UPROPERTY(EditAnywhere, Category = "PlayerToViewPointDistance")
		FVector PlayerToViewPointDistance;
	
public:	
	// Sets default values for this actor's properties
	ACharacterCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//------------------------スプライン------------------------

public:
	//SplineActorの初期化設定
	AActor* SplineActorInitialize(AActor* _pOwnerActor, const FName& _tag);

	//スプライン上の現在の位置を取得
	void GetCurrentInfo0nSpline(float _length, FVector& _location, FRotator& _rotation);
	
	UPROPERTY(EditAnywhere, Category = "SubChara")		//サブキャラを動かしたいスプラインのタグ名
		FName m_splineTagName;

	AActor* m_pSplineActor;		//スプラインのActor
	float m_localLength;		//現在スプライン上の位置

	UPROPERTY(EditAnywhere, Category = "SubChara")		//速度
		float m_defaultSpeed;		//速度

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "SubChara")		//速度
		float m_moveDistance;		//速度

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "SubChara")		//速度
		FRotator m_defaultRRRRRRRRRR;		//速度



protected:
	ACharacter* GetPlayer();
};

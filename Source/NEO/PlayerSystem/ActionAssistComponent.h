// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionAssistComponent.generated.h"


UCLASS(ClassGroup = (ActionAssist), meta = (BlueprintSpawnableComponent))
class NEO_API UActionAssistComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// コンストラクタ
	UActionAssistComponent();

	// ヒットストップ処理
	void HitStop(float _speedDuringHitStop, float _stopTime);

	// エフェクトスポーン
	void SpawnEffect(class UNiagaraSystem* _hitParticle, FVector _spawnPos);

	// オーナーをカメラに向ける
	void OwnerParallelToCamera(bool _lookRight);

	// カメラシェイク
	void CameraShake(TSubclassOf<UCameraShakeBase> _shakePattern, float _scale = 1.f);

	// アニメーション再生
	void PlayAnimation(UAnimMontage* _toPlayAnimMontage, FName _startSectionName = "None", float _playRate = 1.f);

	// サウンド再生
	void PlaySound(class USoundBase* _sound_Obj, float StartTime = 0);

	// レイを飛ばす
	bool WallCheck(float _lineLength);

protected:

	// ゲーム開始時に呼び出される処理
	virtual void BeginPlay() override;

	// 毎フレーム呼び出される処理
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	// ヒットストップ終了
	void EndHitStop();


protected:

	//-------------各機能のON・OFF---------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Assist")
		bool bUseCorrectAttackAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Assist")
		bool bUseHitStop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Assist")
		bool bUseHitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Assist")
		bool bUseFaceCamera;

	//----------------------------------------------------------------------------

private:

	// タイマーハンドル
	FTimerHandle TimerHandle_HitStop;

	// アニメーションプレイ中かどうか
	bool IsAnimationPlaying;
};

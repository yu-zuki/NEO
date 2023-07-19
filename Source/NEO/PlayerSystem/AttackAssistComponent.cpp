// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAssistComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UAttackAssistComponent::UAttackAssistComponent()
	: bUseCorrectAttackAngle(true)
	, bUseHitStop(true)
	, bUseHitEffect(true)
{
	PrimaryComponentTick.bCanEverTick = false;

	LineLength = 300.f;
	SpeedDuringHitStop = 0.1f;
	HitStopTime = 0.2f;
}


// Called when the game starts
void UAttackAssistComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UAttackAssistComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// 攻撃の角度修正
void UAttackAssistComponent::CorrectAttackAngle()
{
	// 機能のオン・オフ
	if (!bUseCorrectAttackAngle) { return; }

	//　前方にいるエネミー取得
	AActor* InFrontOfEnemy = GetFrontActor();

	if (InFrontOfEnemy)
	{
		// プレイヤーと敵の方向を取得する
		FVector direction = InFrontOfEnemy->GetActorLocation() - GetOwner()->GetActorLocation();
		direction.Normalize();

		// プレイヤーの向きを取得する
		FRotator CurrentRotation = GetOwner()->GetActorRotation();

		// プレイヤーの向きを変更する
		FRotator NewRotation(CurrentRotation.Pitch, direction.Rotation().Yaw, CurrentRotation.Roll);
		GetOwner()->SetActorRotation(NewRotation);
	}
}

// ヒットストップ処理
void UAttackAssistComponent::HitStop()
{
	// 機能のオン・オフ
	if (!bUseHitStop) { return; }

	//処理内容
	ACharacter* Character = Cast<ACharacter>(GetOwner());		//CharacterGet
	if (Character == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AttackAssitComponent: Character is nullptr"));
		return;
	}

	//HitStopを開始
	Character->GetMesh()->GlobalAnimRateScale = SpeedDuringHitStop;

	//HitStopを停止
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle_HitStop, this, &UAttackAssistComponent::EndHitStop, HitStopTime, false);
}

// ヒットエフェクト
void UAttackAssistComponent::HitEffect()
{
	// 機能のオン・オフ
	if (!bUseHitEffect) { return; }



}

// エフェクトスポーン
void UAttackAssistComponent::SpawnHitEffect()
{

}

// 前方にいるActorを返す
AActor* UAttackAssistComponent::GetFrontActor()
{
	// レイを飛ばす
	// 飛ばす方向指定
	float Rotation_Z = GetOwner()->GetActorRotation().Yaw;
	float LineDirection = (Rotation_Z > 0) ? (LineLength) : (-LineLength);

	// 始点
	FVector start = GetOwner()->GetActorLocation();

	// 終点
	FVector end = FVector(start.X, start.Y + LineDirection, start.Z);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	// 当たったオブジェクト格納用
	FHitResult OutHit;

	// ヒットした場合true
	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, start, end, ECC_WorldStatic, CollisionParams);

	DrawDebugLine(GetWorld(),start,end, FColor::Green, true, 1.0f);

	AActor* HitEnemy = OutHit.GetActor();

	if (isHit && HitEnemy != nullptr)
	{
		return HitEnemy;
	}

	return nullptr;
}

// ヒットストップ終了
void UAttackAssistComponent::EndHitStop()
{
	//CharacterGet
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AttackAssitComponent: Character is nullptr"));
		return;
	}

	//HitStopを停止
	Character->GetMesh()->GlobalAnimRateScale = 1.f;
}

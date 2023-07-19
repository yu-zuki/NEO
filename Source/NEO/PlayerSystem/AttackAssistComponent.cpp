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


/*
 * 関数名　　　　：CorrectAttackAngle()
 * 処理内容　　　：角度を補正して攻撃を当たりやすくする(直線状にいる敵)
 * 戻り値　　　　：なし
 */
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


/*
 * 関数名　　　　：HitStop()
 * 処理内容　　　：ヒットストップを起こす
 * 戻り値　　　　：なし
 */
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


/*
 * 関数名　　　　：SpawnHitEffect()
 * 処理内容　　　：ヒットエフェクトをスポーン
 * 戻り値　　　　：なし
 */
void UAttackAssistComponent::SpawnHitEffect()
{
	// 機能のオン・オフ
	if (!bUseHitEffect) { return; }


}

/*
 * 関数名　　　　：GetFrontActor()
 * 処理内容　　　：敵が直線状にいるか判定
 * 戻り値　　　　：見つけた敵の情報を返す
 */
AActor* UAttackAssistComponent::GetFrontActor()
{
	// 所有者の情報取得
	AActor* pOwner = GetOwner();

	if (pOwner)
	{
		// レイを飛ばす
		// 飛ばす方向指定
		float Rotation_Z = pOwner->GetActorRotation().Yaw;
		float LineDirection = (Rotation_Z > 0) ? (LineLength) : (-LineLength);

		// 始点
		FVector start = pOwner->GetActorLocation();

		// 終点
		FVector end = FVector(start.X, start.Y + LineDirection, start.Z);

		// 自身を除く
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(pOwner);

		// 当たったオブジェクト格納用
		FHitResult OutHit;

		// ヒットした場合true
		bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, start, end, ECC_WorldStatic, CollisionParams);
		DrawDebugLine(GetWorld(), start, end, FColor::Green, true, 1.0f);

		if (isHit)
		{
			AActor* HitEnemy = OutHit.GetActor();

			// "Enemy"タグを持っているActorのみを返す
			if (HitEnemy->ActorHasTag("Enemy"))
			{
				return HitEnemy;
			}
		}
	}

	return nullptr;
}


/*
 * 関数名　　　　：EndHitStop()
 * 処理内容　　　：ヒットストップ終了
 * 戻り値　　　　：なし
 */
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

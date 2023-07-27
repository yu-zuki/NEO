// Fill out your copyright notice in the Description page of Project Settings.


#include "BackGroundAssist.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/PlayerSystem/CharacterCamera.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UBackGroundAssist::UBackGroundAssist()
	: bUseBillboard(true)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UBackGroundAssist::BeginPlay()
{
	Super::BeginPlay();


}


// Called every frame
void UBackGroundAssist::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 常にカメラを向く
	ToFaceCamera();
}

void UBackGroundAssist::ToFaceCamera()
{
	// 機能のオン・オフ
	if (!bUseBillboard) { return; }

	// プレイヤーコントローラー作成
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) { return; }

	// 現在アクティブなカメラ取得
	APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
	if (!CameraManager) { return; }

	UCameraComponent* ActiveCameraComp = Cast<UCameraComponent>(CameraManager->GetViewTarget()->GetComponentByClass(UCameraComponent::StaticClass()));
	if (!ActiveCameraComp) { return; }


	// カメラの現在位置と角度を取得
	const FVector CameraLocation = ActiveCameraComp->GetComponentLocation();
	const FRotator CameraRotation = ActiveCameraComp->GetComponentRotation();


	// カメラの位置や角度が変わっていなかったら計算しない
	if (BeforeCameraPos == CameraLocation && BeforeCameraRot == CameraRotation)
	{
		return;
	}
	// 変わっていれば新しい方向を計算
	else
	{
		// カメラの値保存
		BeforeCameraPos = CameraLocation;
		BeforeCameraRot = CameraRotation;

		// 自身の現在位置と角度を取得
		const FVector nowPos = GetOwner()->GetActorLocation();

		// カメラへの方向ベクトル計算
		const FVector DirectionToCamera = (CameraLocation - nowPos).GetSafeNormal();

		// 回転取得
		const FRotator newRot = DirectionToCamera.Rotation();

		// 新しい角度反映
		GetOwner()->SetActorRotation(FRotator(newRot.Pitch, newRot.Yaw - 90.f, newRot.Roll));
	}
}
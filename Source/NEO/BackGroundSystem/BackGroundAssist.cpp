// Fill out your copyright notice in the Description page of Project Settings.


#include "BackGroundAssist.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/PlayerSystem/CharacterCamera.h"
#include "Camera/CameraComponent.h"
#include "NEO/GameSystem/TGS_GameMode.h"

#define DIRECTION (90.0)

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

	// ��ɃJ����������
	ToFaceCamera();
}

void UBackGroundAssist::ToFaceCamera()
{
	// �@�\�̃I���E�I�t
	if (!bUseBillboard) { return; }

	// �v���C���[�R���g���[���[�쐬
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) { return; }

	// ���݃A�N�e�B�u�ȃJ�����擾
	APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
	if (!CameraManager) { return; }

	UCameraComponent* ActiveCameraComp = Cast<UCameraComponent>(CameraManager->GetViewTarget()->GetComponentByClass(UCameraComponent::StaticClass()));
	if (!ActiveCameraComp) { return; }

	// �J�����̌��݈ʒu�Ɗp�x���擾
	ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode) { return; }
	const FVector CameraLocation = GameMode->GetCameraLocation();



	// �J�����̈ʒu��p�x���ς���Ă��Ȃ�������v�Z���Ȃ�
	if (BeforeCameraPos == CameraLocation)
	{
		return;
	}
	// �ς���Ă���ΐV�����������v�Z
	else
	{
		// �J�����̒l�ۑ�
		BeforeCameraPos = CameraLocation;

		// ���g�̌��݈ʒu�Ɗp�x���擾
		const FVector nowPos = GetOwner()->GetActorLocation();

		// �J�����ւ̕����x�N�g���v�Z
		const FVector DirectionToCamera = (CameraLocation - nowPos).GetSafeNormal();

		// ��]�擾
		FRotator newRot = DirectionToCamera.Rotation();

		//float newRot = FMath::Atan2(DirectionToCamera.Z, DirectionToCamera.Y);


		// �V�����p�x���f
		GetOwner()->SetActorRotation(FRotator(0.0,newRot.Yaw - DIRECTION,0.0));
	}
}
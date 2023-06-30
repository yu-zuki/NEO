// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPOdaEnum.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "OdaBase.generated.h"

UCLASS()
class NEO_API AOdaBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOdaBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//�{�X�L�����N�^�[
	UPROPERTY()
	AOdaBase* OdaNobunaga;

	//�L�����N�^�[���[�u�����g
	UPROPERTY()
		UCharacterMovementComponent* NobunagaMovement;
	
	//�O�������Ƃ�
	UPROPERTY()
	FVector ForwardDirection;

	//�񋓌^
	ECPPOdaEnum OdaMoveEnum;

	//�ҋ@�֐�
	UPROPERTY()
	bool SwitchStayMove;

	int WaitTime;

	void OdaStay1(int Timer);

	void OdaMove1(int Timer);

	void OdaMove2(int Timer);

	void OdaBack1(int Timer);


	//�ʏ�̈ړ����x
	UPROPERTY(EditAnywhere)
		float OdaSpeed;

	//�}�ȑ��x
	UPROPERTY(EditAnywhere)
		float FastOdaSpeed;

	//�O���ړ��ύX�J�E���^�[
	UPROPERTY()
		int FlontTimer;
	
	//�s���ύX���Ԑݒ�
	UPROPERTY(EditAnywhere)
		int ChangeFlontTimer;

	//�O���ړ�
	UFUNCTION()
		void FlontMove(float Speed);

	//����ړ�
	UFUNCTION()
		void BackMove(float Speed);
};

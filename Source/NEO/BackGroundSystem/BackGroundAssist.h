// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BackGroundAssist.generated.h"


UCLASS( ClassGroup=(BackGroundAssist), meta=(BlueprintSpawnableComponent) )
class NEO_API UBackGroundAssist : public UActorComponent
{
	GENERATED_BODY()

public:	

	// �R���X�g���N�^
	UBackGroundAssist();

protected:

	// �Q�[���J�n���ɌĂяo����鏈��
	virtual void BeginPlay() override;

public:	

	// ���t���[���Ăяo����鏈��
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	// ��ɃJ�����̕�������������
	void ToFaceCamera();

protected:

	// �@�\���g�����ǂ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Assist")
		bool bUseBillboard;


		
private:
	FVector BeforeCameraPos;
};

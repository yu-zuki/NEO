// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerNotify.generated.h"

UCLASS()
class NEO_API ATriggerNotify : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerNotify();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game starts or when spawned
	virtual void Tick(float DeltaTime) override;

	// �ڐG�J�n���ɍs������
	UFUNCTION()
		virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:

	// �����������Ƃ�m�点��
	UFUNCTION()
		bool IsEnter() const { return IsEnterBox; }

private:

	// ����p�̃{�b�N�X�R���|�[�l���g
	class UBoxComponent* pTriggerBoxComp;

	// �R���W�����ɓ������Ă��邩�i�[
	bool IsEnterBox;
};

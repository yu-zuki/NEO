// Fill out your copyright notice in the Description page of Project Settings.


#include "CurveTrigger.h"
#include "Components/BoxComponent.h"
#include "PlayerBase.h"

// Sets default values
ACurveTrigger::ACurveTrigger()
{
	// Tick()�𖈃t���[���Ă΂Ȃ��悤�ɂ���
	PrimaryActorTick.bCanEverTick = false;

	// �^�O�ݒ�
	Tags.Add("CurveTrigger");

	CurveTriggerComp = CreateDefaultSubobject<UBoxComponent>("TriggerCollision");

	/*OnActorBeginOverlap.AddDynamic(this, &ACurveTrigger::OnActorOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ACurveTrigger::OnActorOverlapEnd);*/
}

// Called when the game starts or when spawned
void ACurveTrigger::BeginPlay()
{
	Super::BeginPlay();

}

//// �g���K�[�ɓ���������J�[�u�J�n
//void ACurveTrigger::OnActorOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
//{
//	// �q�b�g�����A�N�^�[��"Player"�^�O�������Ă�����
//	if (OtherActor && (OtherActor != this) && OtherActor->ActorHasTag("Player"))
//	{
//		// �v���C���[�̏��擾
//		APlayerBase* pPlayer = Cast<APlayerBase>(OtherActor);
//
//		// �J�[�u�ɓ��������Ƃ�m�点��
//		if (pPlayer)
//		{
//			//pPlayer->
//		}
//	}
//}
//
//void ACurveTrigger::OnActorOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
//{
//	// �q�b�g�����A�N�^�[��"Player"�^�O�������Ă�����
//	if (OtherActor && (OtherActor != this) && OtherActor->ActorHasTag("Player"))
//	{
//		// �J�[�u���I���������Ƃ��v���C���[�ɒm�点��
//		//EnterCurve = false;
//	}
//}

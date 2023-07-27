// Fill out your copyright notice in the Description page of Project Settings.


#include "CurveTrigger.h"
#include "Components/BoxComponent.h"
#include "PlayerBase.h"

// Sets default values
ACurveTrigger::ACurveTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �^�O�ݒ�
	Tags.Add("CurveTrigger");

	CurveTriggerComp = CreateDefaultSubobject<UBoxComponent>("TriggerCollision");

	OnActorBeginOverlap.AddDynamic(this, &ACurveTrigger::OnActorOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ACurveTrigger::OnActorOverlapEnd);
}

// Called when the game starts or when spawned
void ACurveTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACurveTrigger::OnActorOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// �q�b�g�����A�N�^�[��"Player"�^�O�������Ă�����
	if (OtherActor && (OtherActor != this) && OtherActor->ActorHasTag("Player"))
	{
		// �v���C���[���i�[
		APlayerBase* Player = Cast<APlayerBase>(OtherActor);

		// �J�[�u�ɓ˓��������Ƃ��v���C���[�ɒm�点��
		Player->OnCurveMode(true);
	}
}

void ACurveTrigger::OnActorOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	// �q�b�g�����A�N�^�[��"Player"�^�O�������Ă�����
	if (OtherActor && (OtherActor != this) && OtherActor->ActorHasTag("Player"))
	{
		// �v���C���[���i�[
		APlayerBase* Player = Cast<APlayerBase>(OtherActor);

		// �J�[�u���I���������Ƃ��v���C���[�ɒm�点��
		Player->OnCurveMode(false);
	}
}

// Called every frame
void ACurveTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



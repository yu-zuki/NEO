// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerNotify.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATriggerNotify::ATriggerNotify()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	pTriggerBoxComp = CreateDefaultSubobject<UBoxComponent>("TriggerBoxComp");

	RootComponent = pTriggerBoxComp;

	if (pTriggerBoxComp != NULL)
	{
		pTriggerBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ATriggerNotify::OnOverlap);
	}
}

// Called when the game starts or when spawned
void ATriggerNotify::BeginPlay()
{
	Super::BeginPlay();

	// �I�[�o�[���b�v�ڐG���n�߂����ɌĂ΂��C�x���g�֐���o�^
}

// Called every frame
void ATriggerNotify::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



// �I�[�o�[���b�v���n�߂����̏����i�I�[�o�[���C�h�j
void ATriggerNotify::OnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �I�[�o�[���b�v�����ۂɎ��s�������C�x���g
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// �I�[�o�[���b�v�����̂��v���C���[�̎��̂ݔ�����������
		// PlayerCharaBP -> Actor -> Tags�ɐݒ肵���^�O�uPlayer�v�ŁA�v���C���[�����ʂ���
		if (OtherActor->ActorHasTag("Player"))
		{
			//// �f�o�b�O�m�F�p
			// UE_LOG(LogTemp,Warning,TEXT("Goal"));

			// �ڐG�t���O���グ��
			IsEnterBox = true;
		}
	}
}
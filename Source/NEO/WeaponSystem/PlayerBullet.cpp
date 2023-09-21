// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBullet.h"
#include "NEO/Enemy/EnamyBase.h"
#include "NEO/OdaBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APlayerBullet::APlayerBullet()
	: MoveSpeed(20.f)
	, TimeToDelete(5.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �Ή�����I�u�W�F�N�g����
	BulletCollision = CreateDefaultSubobject<UCapsuleComponent>("BulletCollision");

	// ���[�g�ɐݒ�
	if (BulletCollision)
	{
		// ����̃��b�V���ɒǏ]
		BulletCollision->SetupAttachment(RootComponent);
	}

	// ����̃R���|�[�l���g���쐬
	BulletStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletStaticMesh");

	if (BulletStaticMesh)
	{
		// �e�ۂ̃A�Z�b�g�ݒ�
		ConstructorHelpers::FObjectFinder< UStaticMesh > weaponMesh(TEXT("/Game/0122/Player/Weapon/Cartrige/Cartrige"));

		if (weaponMesh.Succeeded())
		{
			BulletStaticMesh->SetStaticMesh(weaponMesh.Object);
			BulletStaticMesh->SetupAttachment(BulletCollision);
		}
	}


	// �R���W�����C�x���g��ݒ�
	BulletCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerBullet::OnOverlap);
}

// Called when the game starts or when spawned
void APlayerBullet::BeginPlay()
{
	Super::BeginPlay();
	
	// �w��̎��ԂŒe�ۂ��폜
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(DeleteTimerHandle, this, &APlayerBullet::SetDestroyFlag, TimeToDelete, false);
}

// Called every frame
void APlayerBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ��ɑO�i
	Move();
}


void APlayerBullet::Move()
{
	// �ړ������
	const FVector MoveValue = FVector(MoveSpeed, 0.f, 0.f);

	// �ړ��l���f
	AddActorLocalOffset(MoveValue);
}


// �폜�܂ł̎��ԂɒB���Ă�����
void APlayerBullet::SetDestroyFlag()
{
	// Tick������؂�
	PrimaryActorTick.bCanEverTick = false;

	// �폜�̎��ԂɒB�������Ƃ�`���邽�߂̃t���O
	const bool DestroyFlag = true;

	// �폜
	DestroyBullet(DestroyFlag);
}

// �e��Destroy
void APlayerBullet::DestroyBullet(bool _destroyFlag /*= false*/)
{
	// �폜�ʒm���������폜
	if (_destroyFlag)
	{
		Destroy();
	}
}


void APlayerBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �I�[�o�[���b�v�����ۂɎ��s�������C�x���g
	if (OtherActor && (OtherActor != this))
	{
		// ���������̂�Enemy�̎��_���[�W�������s��
		if (OtherActor->ActorHasTag("Enemy"))
		{
			AEnamyBase* pEnemy = Cast<AEnamyBase>(OtherActor);
			AOdaBase* pBoss = Cast<AOdaBase>(OtherActor);

			if (pEnemy)
			{
				pEnemy->ApplyDamage(50);
			}
			else if (pBoss)
			{
				pBoss->ApplyDamage(50);
			}

			// �e�ۍ폜
			DestroyBullet(true);
		}
	}
}
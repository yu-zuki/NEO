// Fill out your copyright notice in the Description page of Project Settings.


#include "OdaBase.h"
#include "GameSystem/TGS_GameMode.h"
#include "GameSystem/EnemyBase_WidgetComponent.h"



// Sets default values
//�R���X�g���N�^+�ϐ��̏�����
AOdaBase::AOdaBase():
	OdaNobunaga(this),
	BoxComp(NULL),
	FlameCounter(0),
	OdaMoveEnum(ECPPOdaEnum::Stay1),
	SwitchStayMove(true),
	WaitTime(0),
	OdaSpeed(1.f),
	FastOdaSpeed(1.65),
	ChangeFlontTimer(20),
	isMotionPlaying(false),
	isShockWaveSpawnTiming(false),
	Health(100),
	MaxHealth(100.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordComponent"),true);
	BoxComp->SetupAttachment(GetMesh(),"weapon_r");

	//UI Create
	EnemyWidget = CreateDefaultSubobject<UEnemyBase_WidgetComponent>(TEXT("EnemyWidget"));
	EnemyWidget->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AOdaBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultController();

	NobunagaMovement = (GetCharacterMovement());
	ForwardDirection = (GetActorForwardVector());

	
}


// Called to bind functionality to input
void AOdaBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Called every frame
void AOdaBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Enemy Hp Set
	EnemyWidget->SetHPInfo(Health, MaxHealth);

	//�t���[�����Ƃɉ��Z����
	FlameCounter++;
	//�������v���C���[�̕��Ɍ�����(60�t���[�����ɍX�V)
	if (FlameCounter % 60 == 0)
	{
		//�v���C���[�̕�������
		ToPlayerRotate();
	}
	//�{�X�̑O�������
	ForwardDirection = GetActorForwardVector();
	//�J�E���^�[�N��
	WaitTime++;
	//UKismetSystemLibrary::PrintString(this, FString::FromInt(WaitTime), true, true, FColor::Cyan, 2.f, TEXT("None"));

	

		//��Ԃ��Ƃɓ�����؂�ւ���
		switch (OdaMoveEnum)
		{
			//�ҋ@
		case ECPPOdaEnum::Stay1:
			OdaStay1(WaitTime);
			break;

			//�����P
		case ECPPOdaEnum::Move1:
			OdaMove1(WaitTime);
			break;

			//�����Q
		case ECPPOdaEnum::Move2:
			OdaMove2(WaitTime);
			break;		

			//�ދp
		case ECPPOdaEnum::Back1:
			OdaBack1(WaitTime);
			break;

			//�U���P
		case ECPPOdaEnum::Attack1:
			OdaAttack1(WaitTime);
			break;

			//�U���Q
		case ECPPOdaEnum::Attack2:
			OdaAttack2(WaitTime);
		default:

			break;
		}

}
//Y�����������Ăǂ������ɂ��邩
void AOdaBase::ToPlayerRotate()
{
	//�{�X���v���C���[��葽���Ƃ�
	if (this->GetActorLocation().Y > UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
	{
		//y��270�Ɍ�����(��)
		this->SetActorRotation(FRotator(0.f, 270.f, 0.f));
	}
	else
	{
		this->SetActorRotation(FRotator(0.f, 90.f, 0.f));
	}
}
void AOdaBase::OdaStay1(int Timer)
{
	//���[�V�����𗬂���Ԃ��ǂ���(isMotionPlaying��true�ɂ���)
	if (isMotionPlaying != true)
	{
		isMotionPlaying = true;
	}
	//�v���C���[�Ƃ̋�����400����邩60�t���[����������
	if (GetDistanceTo(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) >= 400 && WaitTime >= 60)
	{
		//�񕪈�
		
		if (FMath::RandRange(0, 1) == 0)
		{
			//�����P��
			OdaMoveEnum = ECPPOdaEnum::Move1;
			SwitchStayMove = false;
		}
		else
		{
			//�����Q��
			OdaMoveEnum = ECPPOdaEnum::Move2;
			SwitchStayMove = false;
		}
	}
}

void AOdaBase::OdaMove1(int Timer)
{
	//2�b�����v���C���[��Y���������ɂȂ�����؂�ւ���
	if (Timer % 120 == 0 || FMath::IsNearlyEqual
	(this->GetActorLocation().Y,UGameplayStatics::GetPlayerPawn(GetWorld(),0)->GetActorLocation().Y,400.f))
	{
		//���]
		SwitchStayMove = true;
	}
	//�l�߂�
	if (SwitchStayMove)
	{
		//�������������Ăяo��
		BossMove(FastOdaSpeed*2, ForwardDirection);
	}
	//�������킹��
	else
	{ 
		//�ǂ������ɂ��邩(�O�����ɑ���)
		if (this->GetActorLocation().X < UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().X)
		{
			BossMove(FastOdaSpeed * 2, FVector(1.f,0.f,0.f));
		}
		else
		{
			BossMove(FastOdaSpeed * 2, FVector(-1.f, 0.f, 0.f));
		}
	}
	//�v���C���[�Ƃ̋������߂�������
	if (GetDistanceTo(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) <= 300.f) {
		OdaMoveEnum = ECPPOdaEnum::Attack1;
		SwitchStayMove = false;
		WaitTime = 0;
	}
}

void AOdaBase::OdaMove2(int Timer)
{
	//2�b�����v���C���[��Y���������ɂȂ�����؂�ւ���
	if (Timer % 120 == 0 || FMath::IsNearlyEqual
	(this->GetActorLocation().Y, UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y, 400.f))
	{
		//���]
		SwitchStayMove = true;
	}
	//�Ռ��g��ł�(�Ռ��g���̂̓u���[�v�����g��)
	if (SwitchStayMove)
	{
		OdaMoveEnum = ECPPOdaEnum::Attack2;
		SwitchStayMove = false;
		WaitTime = 0;
	}
	//�������킹��
	else
	{
		//�ǂ������ɂ��邩
		if (this->GetActorLocation().X < UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().X)
		{
			BossMove(FastOdaSpeed * 2, FVector(1.f, 0.f, 0.f));
		}
		else
		{
			BossMove(FastOdaSpeed * 2, FVector(-1.f, 0.f, 0.f));
		}
	}
}

//������ƌ��ɉ�������
void AOdaBase::OdaBack1(int Timer) {
	//50�t���[����ҋ@�ɖ߂�
	if (Timer % 50 == 0)
	{
		OdaMoveEnum = ECPPOdaEnum::Stay1;
		SwitchStayMove = false;
		WaitTime = 0;
	}
	//���ɉ�����
	BackMove(FastOdaSpeed * 10.f);
}

//�U���P
void AOdaBase::OdaAttack1(int Timer) {
	if (isMotionPlaying == true)
	{
		//�A�j���[�V�����𗬂�(���͉�)
		PlayAnimMontage(AnimMontage_BossAttack1);
		//��x�������������̂Ńt���O��؂�ւ���
		isMotionPlaying = false;
	}

	//150�t���[����������
	if (Timer % 150 == 0)
	{
		//�X�e�[�g��؂�ւ���
		OdaMoveEnum = ECPPOdaEnum::Stay1;
		//�؂�ւ���ɂ������ĕϐ�������������
		SwitchStayMove = false;
		WaitTime = 0;
	}
}

//�U���Q
void AOdaBase::OdaAttack2(int Timer) {
	if (isMotionPlaying == true)
	{
		//�A�j���[�V�����𗬂�(���͉�)
		PlayAnimMontage(AnimMontage_BossAttack2);
		//��x�������������̂Ńt���O��؂�ւ���
		isMotionPlaying = false;
	}


	//�A�j���[�V�����̍Ō�ɏՌ��g���o�������̂�Notify���g���Đ��䂷��(ShockWaveSpawnFlagChange�ɂĕϐ��̒��g��ύX)
	if (isShockWaveSpawnTiming == true)
	{
		//�A�N�^�[�̃X�|�[������(ShockWaveSpawn�̓u���[�v�����g��Őݒ�)
		GetWorld()->SpawnActor<AActor>(ShockWaveSpawn, GetActorTransform());
		//��x�����X�|�[�����������̂Ő؂�ւ��Ă���
		isShockWaveSpawnTiming = false;
	}

	//150�t���[����������
	if (Timer % 150 == 0)
	{
		//�X�e�[�g��؂�ւ���
		OdaMoveEnum = ECPPOdaEnum::Stay1;
		//�؂�ւ���ɂ������ĕϐ�������������
		SwitchStayMove = false;
		WaitTime = 0;
	}
}


//AnimNotify�ɂĕύX�A�U���̃t���O��on,off
void AOdaBase::AttackFlagChange()
{
	//���]������
	IsAttackNow = !IsAttackNow;
}

//���ꂪ�Ă΂ꂽ��Ռ��g���X�|�[�������邽�߂̕ϐ���؂�ւ���
void AOdaBase::ShockWaveSpawnFlagChange()
{
	//�Ռ��g���o��������ׂ̕ϐ���true�ɕς���
	isShockWaveSpawnTiming = true;
	//����IsAttackNow��true�ɂȂ��Ă�����
	if (IsAttackNow == true)
	{
		//false�ɂ��Ă݂�
		IsAttackNow = false;
	}
}


void AOdaBase::ApplyDamage(float Damage)
{
	if (isHPLock != true)
	{
		//�m�b�N�o�b�N�̃A�j���[�V�����𗬂�
		PlayAnimMontage(AnimMontage_BossKnockMontage);
		Health -= Damage;
		//HP��0�ɂȂ�����
		if (Health <= 0.f)
		{
			//�̗͂�0�ȉ��ɂȂ����Ƃ��̏���
			Death();
		}
	}
}

void AOdaBase::HPLock()
{
	//HP���b�N�p�̃X�C�b�`��؂�ւ���
	isHPLock = !isHPLock;
}

//�����֘A
void AOdaBase::BossMove(float Speed, FVector MoveSize)
{
	AddMovementInput(MoveSize, Speed);
}

void AOdaBase::BackMove(float Speed)
{
	AddMovementInput(ForwardDirection, -Speed);
}

void AOdaBase::Death()
{
	ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->DestroyEnemy(this, IsAreaEnemy);
	}
}

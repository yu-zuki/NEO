// Fill out your copyright notice in the Description page of Project Settings.


#include "OdaBase.h"
#include "GameSystem/TGS_GameMode.h"
#include "GameSystem/EnemyBase_WidgetComponent.h"



// Sets default values
//�R���X�g���N�^+�ϐ��̏�����
AOdaBase::AOdaBase() :
	BoxComponent(NULL),
	FlameCounter(0),
	OdaMoveEnum(ECPPOdaEnum::Stay1),
	SwitchStayMove(true),
	WaitTime(0),
	OdaSpeed(1.f),
	FastOdaSpeed(1.65),
	ChangeFlontTimer(20),
	isMotionPlaying(false),
	isShockWaveSpawnTiming(false),
	isUltShot(false),
	UltTimer(0),
	isNotAttackNow(false),
	NotAttackCount(0),
	SwordDamage(10),
	bIsAttacked(false),
	Health(150),
	MaxHealth(150.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordComponent"),true);
	BoxComponent->SetupAttachment(GetMesh(),"weapon_r");
	BoxComponent->SetRelativeLocation(FVector(0.0f, -80.0f, 0.0f));

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
	PlayerChara = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
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

	//���������
	//X��
	BossPosX = FVector(GetActorLocation().X, 0.f, 0.f);
	PlayerPosX = FVector(PlayerChara->GetActorLocation().Y, 0.f, 0.f);

	//Y��
	BossPosY = FVector(0.f, GetActorLocation().Y, 0.f);
	PlayerPosY = FVector(0.f, PlayerChara->GetActorLocation().Y, 0.f);

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

			//�K�E�Z
		case ECPPOdaEnum::Ultimate:
			OdaUlt(WaitTime);

			break;
		}

}
//Y�����������Ăǂ������ɂ��邩
void AOdaBase::ToPlayerRotate()
{
	//�{�X���v���C���[��葽���Ƃ�
	if (GetActorLocation().Y > UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
	{
		//y��270�Ɍ�����(��)
		SetActorRotation(FRotator(0.f, 270.f, 0.f));
	}
	else
	{
		SetActorRotation(FRotator(0.f, 90.f, 0.f));
	}
}
void AOdaBase::OdaStay1(int Timer)
{
	UKismetSystemLibrary::PrintString(this, "Stay", true, true, FColor::Cyan, 2.f, TEXT("None"));

	//���[�V�����𗬂���Ԃ��ǂ���(isMotionPlaying��true�ɂ���)
	if (isMotionPlaying != true)
	{
		isMotionPlaying = true;
	}
	//HP��50%�ȉ��ɂȂ�����

	if (Health < MaxHealth/2.f)
	{
		if (isUltShot == false)
		{
			//�K�E�Z
			OdaMoveEnum = ECPPOdaEnum::Ultimate;
			isUltShot = true;
		}
		else
		{
			UltTimer++;
			if (UltTimer % 600 == 0)
			{
				isUltShot = false;
			}
		}
	}


	//�v���C���[�Ƃ̋����̔���
	if (FVector::Dist(BossPosY, PlayerPosY) <= 200.f)
	{
		//�ߐڍU��
		OdaMoveEnum = ECPPOdaEnum::Attack1;
		WaitTime = 0;
	}
	if (FVector::Dist(BossPosX, PlayerPosX) <= 50.f)
	{

		if(FVector::Dist(BossPosY, PlayerPosY) >= 300.f)
		{
			//������
			OdaMoveEnum = ECPPOdaEnum::Attack2;
			WaitTime = 0;
		}
	}
	else
	{
		//if (Timer % 30 == 0)
		{
			//if (FMath::RandRange(0, 100) <= 20)
			{
				//�������킹�ɍs��
				if (GetActorLocation().X < UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().X)
				{
					BossMove(FastOdaSpeed * 2, FVector(1.f, 0.f, 0.f));
				}
				else
				{
					BossMove(FastOdaSpeed * 2, FVector(-1.f, 0.f, 0.f));
				}
			}
		}
	}
}

//������ƌ��ɉ�������
void AOdaBase::OdaBack1(int Timer) {
	UKismetSystemLibrary::PrintString(this, "Back", true, true, FColor::Cyan, 2.f, TEXT("None"));
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
	UKismetSystemLibrary::PrintString(this, "Attack1", true, true, FColor::Cyan, 2.f, TEXT("None"));


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
		//���Z�b�g
		NotAttackCount = 0;
		//�m�b�N�o�b�N���ɍU�����[�V�����ɓ����HP���b�N���쓮�������Ă��܂��̂ł����Ő؂�ւ���
		if (isHPLock == true)
		{
			HPLock();
		}
	}
}

//�U���Q
void AOdaBase::OdaAttack2(int Timer) {
	UKismetSystemLibrary::PrintString(this, "Attack2", true, true, FColor::Cyan, 2.f, TEXT("None"));
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
		//���Z�b�g
		NotAttackCount = 0;
		//�m�b�N�o�b�N���ɍU�����[�V�����ɓ����HP���b�N���쓮�������Ă��܂��̂ł����Ő؂�ւ���
		if (isHPLock == true)
		{
			HPLock();
		}
	}
}
void AOdaBase::OdaUlt(int Timer)
{
	UKismetSystemLibrary::PrintString(this, "Ult", true, true, FColor::Cyan, 2.f, TEXT("None"));

	if (Timer % 80 == 0)
	{
		if (isMotionPlaying == true)
		{
			//�A�j���[�V�����𗬂�(���͉�)
			PlayAnimMontage(AnimMontage_BossUltimate);
			//��x�������������̂Ńt���O��؂�ւ���
			isMotionPlaying = false;
		}
	//�A�j���[�V�����̍Ō�ɕK�E�Z���o�������̂�Notify���g���Đ��䂷��(UltSpawnFlagChange�ɂĕϐ��̒��g��ύX)

		if (isUltSpawnTiming == true)
		{
			//�A�N�^�[�̃X�|�[������(UltSpawn�̓u���[�v�����g��Őݒ�)
			GetWorld()->SpawnActor<AActor>(UltSpawn, GetActorTransform());
			//��x�����X�|�[�����������̂Ő؂�ւ��Ă���
			isUltSpawnTiming = false;
		}
	}


	//200�t���[����������
	if (Timer % 200 == 0)
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

//���ꂪ�Ă΂ꂽ��K�E�Z���X�|�[�������邽�߂̕ϐ���؂�ւ���
void AOdaBase::UltSpawnFlagChange()
{
	//�K�E�Z���o��������ׂ̕ϐ���true�ɕς���
	isUltSpawnTiming = true;
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
		if (NotAttackCount < 5)
		{
			//�^�C�}�[�����Z�b�g
			WaitTime = 0;
			NotAttackCount++;
		}

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
	//(��)���ʕ����ɕ�������
	AddMovementInput(MoveSize, Speed);
}

void AOdaBase::BackMove(float Speed)
{
	AddMovementInput(FVector(-1.f,0.f,0.f), -Speed);
}


void AOdaBase::toPlayerAttacked()
{
	if (bIsAttacked)
	{
		bIsAttacked = false;
	}
}


void AOdaBase::Death()
{
	//�Q�[�����[�h�ɂĂ��̃A�N�^����������
	ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->DestroyEnemy(this, IsAreaEnemy);
	}
}

//�`�F�b�N
void AOdaBase::CheckOverlap()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	FVector Start = BoxComponent->GetComponentLocation();
	FVector End = Start;// + GetActorForwardVector() * 100.0f;
	FQuat Rot = BoxComponent->GetComponentQuat();			// 
	FCollisionShape CollisionBox = FCollisionShape::MakeBox(BoxComponent->GetUnscaledBoxExtent());

	bool isHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, Rot, ECollisionChannel::ECC_GameTraceChannel1, CollisionBox, CollisionParams);

	if (isHit != true) { return; }

	if (false) {
		DrawDebugCapsule(GetWorld(), (Start + End) / 2,
			CollisionBox.GetCapsuleHalfHeight(),
			CollisionBox.GetCapsuleRadius(), Rot, FColor::Red, true, -1.0f, 0, 1.0f);
	}

	for (FHitResult HitResult : HitResults) {
		EnemyOnOverlap(HitResult);
	}
}

void AOdaBase::EnemyOnOverlap(FHitResult& _HitResult)
{
	//Cast
	APlayerBase* Player = Cast<APlayerBase>(_HitResult.GetActor());
	if (Player) {
		//�v���C���[��HP�����b�N�����炱�̏�����ʂ�
		if (bIsAttacked) {
			return;
		}

		Player->TakedDamage(SwordDamage);						//�v���C���[�Ƀ_���[�W��^����

		//���Z�b�g
		bIsAttacked = true;

		FVector HitLocation = _HitResult.Location;		//�q�b�g�G�t�F�N�g�̈ʒu
	}
}

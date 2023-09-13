// Fill out your copyright notice in the Description page of Project Settings.


#include "OdaBase.h"
#include "GameSystem/TGS_GameMode.h"
#include "GameSystem/EnemyBase_WidgetComponent.h"



// Sets default values
//�R���X�g���N�^+�ϐ��̏�����
AOdaBase::AOdaBase() :
	SpawnTimer(0),						//---------------------------
	FlameCounter(0),					//�^�C�}�[�n
	WaitTime(0),						//---------------------------
	isMove(true),
	OdaMoveEnum(ECPPOdaEnum::Stay1),	//�񋓌^
	Attack1Delay(0),					//�ߐڍU���̑ҋ@����
	RandomNum(-1),
	BladeDamage(0.f),
	ShockWaveDamage(10.f),
	UltShockWaveDamage(5.f),
	UltDamage(30.f),
	OdaSpeed(1.f),
	OneMoreShockWave(false),
	ChangeFlontTimer(200),
	isMotionPlaying(false),
	isShockWaveSpawnTiming(false),
	isUltShotTiming(false),
	isUltShot(true),
	UltTimer(550),
	isNotAttackNow(false),
	NotAttackCount(0),
	Attack1WaitTimer(0),
	Attack1WaitingTime(60),
	SwordFirstDamage(5),
	HoldDamageAdd(0),
	SwordAddDamage(5),
	bIsAttacked(false),
	Health(1000.f),
	MaxHealth(1000.f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	swordConp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"), true);
	swordConp->SetupAttachment(GetMesh(), "R_Weapon");


	//Box Create
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordComponent"), true);
	BoxComp->SetupAttachment(GetMesh(), "R_Weapon");
	BoxComp->SetRelativeLocation(FVector(0.0f, 0.f, 0.0f));
	BoxComp->SetBoxExtent(FVector(10.f, 70.f, 10.f));

	//UI Create
	EnemyWidget = CreateDefaultSubobject<UEnemyBase_WidgetComponent>(TEXT("EnemyWidget"));
	EnemyWidget->SetupAttachment(RootComponent);
	// Assist Create
	ActionAssistComp = CreateDefaultSubobject<UActionAssistComponent>(TEXT("ActionAssist"));

}

// Called when the game starts or when spawned
void AOdaBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultController();

	NobunagaMovement = (GetCharacterMovement());
	PlayerChara = nullptr;

	//�f�B���C����莞�Ԃ��U�����󂯂��珈����؂�ւ���
	SpawnDelay = true;

	//
	isBossHPRock = true;
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

	//�v���C���[�����X�|�[���������ɒ��g���Ȃ��Ȃ��Ă��܂��̂ōX�V
	AActor* Player = GetPlayer();
	//�v���C���[���Ȃ������炱��ȍ~�̏������X���[����
	if (Player == nullptr)  return;

	if (FVector::Dist(GetActorLocation(), UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation()) <= 1000.f)
	{

		//Enemy Hp Set
		EnemyWidget->SetHPInfo(Health, MaxHealth);

		//�t���[�����Ƃɉ��Z����
		FlameCounter++;


		if (isMove == false)
		{
			return;
		}
		//�������v���C���[�̕��Ɍ�����(60�t���[�����ɍX�V)
		if (FlameCounter % 60 == 0)
		{
			//�v���C���[�̕�������
			ToPlayerRotate();
		}

		//���������
		//X��
		BossPosX = FVector(GetActorLocation().X, 0.f, 0.f);
		PlayerPosX = FVector(Player->GetActorLocation().X, 0.f, 0.f);

		//Y��
		BossPosY = FVector(0.f, GetActorLocation().Y, 0.f);
		PlayerPosY = FVector(0.f, Player->GetActorLocation().Y, 0.f);

		//�J�E���^�[�N��
		WaitTime++;
		if (isAttack1Waiting)
		{
			Attack1Wait();
		}

		//�̗͂������ȉ��ɂȂ�����
		if (Health < MaxHealth / 2.f)
		{
			//���ڂ������߂̃^�C�}�[���N��
			UltTimer++;
			if (UltTimer % 600 == 0)//600�t���[����ɕK�E�����Ă�悤�ɂ���
			{
				//�K�E�Z�����Ă�悤�ɂ���
				isUltShot = false;
			}
		}

		//��Ԃ��Ƃɓ�����؂�ւ���
		switch (OdaMoveEnum)
		{
			//�ҋ@,����
		case ECPPOdaEnum::Stay1:
			OdaStay1(WaitTime);
			break;
			//���ʃ_�b�V��
		case ECPPOdaEnum::Moveflont:
			if (GetActorLocation().Y < UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
			{
				BossMove(OdaSpeed * 2, FVector(0.f, 3.f, 0.f));
			}
			else
			{
				BossMove(OdaSpeed * 2, FVector(0.f, -3.f, 0.f));
			}			
			if (WaitTime % 25 == 0)
			{
				OdaMoveEnum = ECPPOdaEnum::Stay1;
			}

			//�{�X�̓������~�߂�
		case ECPPOdaEnum::Stop:
			//��{�͕ʂ̂Ƃ���ŉ������邪����Ȃ������ꍇ�����I�ɉ�������
			if (WaitTime % 120 == 0)
			{
				//�ҋ@�ɖ߂�
				BacktoStayCase();
			}
			break;

			//���ʃ_�b�V��
		case ECPPOdaEnum::Moveflont:
			//�E���������������Ő�����ς���
			if (GetActorLocation().Y < UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
			{
				BossMove(OdaSpeed, FVector(0.f, 3.f, 0.f));
			}
			else
			{
				BossMove(OdaSpeed, FVector(0.f, -3.f, 0.f));
			}
			//��莞�Ԃ�������
			if (WaitTime % 25 == 0)
			{
				//�ҋ@�ɖ߂�
				BacktoStayCase();
			}
			break;

			//�w�ʃ_�b�V��
		case ECPPOdaEnum::MoveBack:
			//�E���������������Ő�����ς���
			if (GetActorLocation().Y < UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
			{
				BossMove(OdaSpeed, FVector(0.f, -3.f, 0.f));
			}
			else
			{
				BossMove(OdaSpeed, FVector(0.f, 3.f, 0.f));
			}
			//��莞�Ԃ�������
			if (WaitTime % 60 == 0)
			{
				//�ҋ@�ɖ߂�
				BacktoStayCase();
			}
			break;

			//�U���P
		case ECPPOdaEnum::Attack1:
			if (Attack1WaitTimer % Attack1WaitingTime == 0)
			{
				OdaAttack1(WaitTime);
			}
			else if (WaitTime % Attack1WaitingTime == 0)
			{
				OdaAttack1(WaitTime);
			}
			break;

			//�U���Q
		case ECPPOdaEnum::Attack2:
			OdaAttack2(WaitTime);

			break;

			//�K�E�Z
		case ECPPOdaEnum::Ultimate:
			OdaUlt(WaitTime);
			break;

		default:
			break;
		}
	}
}

/*
 * �֐����@�@�@�@�FToPlayerRotate()
 * �������e�@�@�@�FY�����������Ăǂ������ɂ��邩�𔻒肷��֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::ToPlayerRotate()
{
	bool LookRight;

	//�{�X���v���C���[�̉E�ɂ���Ƃ�
	if (GetActorLocation().Y < UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
	{
		LookRight = true;
	}
	else
	{
		LookRight = false;
	}

	ActionAssistComp->OwnerParallelToCamera(LookRight);
}

/*
 * �֐����@�@�@�@�FAttack1Wait()
 * �������e�@�@�@�F�ߐڍU����������ҋ@����J�E���^�[���N������֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::Attack1Wait()
{
	Attack1WaitTimer++;
}

/*
 * �֐����@�@�@�@�FOdaStay1()
 * �������e�@�@�@�F�U�������ҋ@���Ă���Ƃ��̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::OdaStay1(int Timer)
{
	//���[�V�����𗬂���Ԃ��ǂ���(isMotionPlaying��true�ɂ���)
	if (isMotionPlaying != true)
	{
		isMotionPlaying = true;
	}

	else if (FVector::Dist(BossPosX, PlayerPosX) <= 50.f)
	{

		if (isUltShot == false)
		{
			//�K�E�Z
			OdaMoveEnum = ECPPOdaEnum::Ultimate;
			isUltShot = true;
			UltTimer = 0;

			//random�Œ�̃��Z�b�g
			if (RandomNum != -1)
			{
				RandomNum = -1;
			}
		}
		//�v���C���[�Ƃ̋����̔���
		else if (FVector::Dist(BossPosY, PlayerPosY) <= 200.f)
		{
			//�ߐڍU��
			OdaMoveEnum = ECPPOdaEnum::Attack1;
			WaitTime = 0;
			//�����U���̃f�B���C�̒l�������Ă��Ȃ�������
			if (Attack1Delay != 0)
			{
				Attack1Delay = 10;
			}

			//random�Œ�̃��Z�b�g
			if (RandomNum != -1)
			{
				RandomNum = -1;
			}
		}
		//����Ă�����
		else if (FVector::Dist(BossPosY, PlayerPosY) >= 300.f)
		{
			//������
			OdaMoveEnum = ECPPOdaEnum::Attack2;
			WaitTime = 0;
			//random�Œ�̃��Z�b�g
			if (RandomNum != -1)
			{
				RandomNum = -1;
			}
		}
		else
		{
			//�������킹�ɍs��
			OdaMove1(Timer, 60);
			Attack1Delay = 15;
		}
		//-----------------------------------------------------------------------------------------------------------------------------------------------

	}
	else
	{
		//�������킹�ɍs��
		if (GetActorLocation().X < UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().X)
		{
			BossMove(OdaSpeed * 2, FVector(1.f, 0.f, 0.f));
		}
		else
		{
			BossMove(OdaSpeed * 2, FVector(-1.f, 0.f, 0.f));
		}

	}
}

/*
 * �֐����@�@�@�@�FOdaMove1()
 * �������e�@�@�@�F�������킹��ȂǓ����ɂ��Ă̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::OdaMove1(int DeltaTime, int StopTimer)
{
	if (isAttack1Waiting)
	{
		isAttack1Waiting = false;
	}

	//�������킹�ɍs��
	//random�̒��ɒl�������Ă��邩(-1�������Ă��Ȃ����)
	if (RandomNum == -1)
	{
		//random��0��1������
		RandomNum = FMath::RandRange(0, 1);
	}

	//�O�����ɓ�������
	if (RandomNum == 0)
	{
		BossMove(OdaSpeed * 2, FVector(0.f, 1.f, 0.f));
	}
	else if (RandomNum == 1)
	{
		BossMove(OdaSpeed * 2, FVector(0.f, -1.f, 0.f));
	}

}
/*
 * �֐����@�@�@�@�FOdaAttack1()
 * �������e�@�@�@�F�ߋ����U�������邽�߂̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::OdaAttack1(int Timer) {
	//UKismetSystemLibrary::PrintString(this, "Attack1", true, true, FColor::Cyan, 2.f, TEXT("None"));
	if (Attack1Delay == 0)
	{
		//0���Ɗ���؂�Ȃ��̂�1�������Ă���
		Attack1Delay = 1;
	}

	//�ݒ肵�����Ԃ𒴂�����
	if (Timer % Attack1Delay == 0)
	{
		//���[�V����(�A�j���[�V����)���N��������
		if (isMotionPlaying == true)
		{
			//�A�j���[�V�����𗬂�(���͉�)
			PlayAnimMontage(AnimMontage_BossAttack1);
			//��x�������������̂Ńt���O��؂�ւ���
			isMotionPlaying = false;
			//�f�B���C�����Z�b�g����
			Attack1Delay = 0;
		}
	}

	//150�t���[����������
	if (Timer % 150 == 0)
	{
		//�X�e�[�g��؂�ւ���
		BacktoStayCase();
		//�؂�ւ���ɂ������ĕϐ�������������
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


/*
 * �֐����@�@�@�@�FOdaAttack2()
 * �������e�@�@�@�F�������U�������邽�߂̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::OdaAttack2(int Timer) {
	//UKismetSystemLibrary::PrintString(this, "Attack2", true, true, FColor::Cyan, 2.f, TEXT("None"));

	//���[�V����(�A�j���[�V����)���N��������
	if (isMotionPlaying == true)
	{
		//�_���[�W�l����
		BladeDamage = ShockWaveDamage;
		//�A�j���[�V�����𗬂�
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

	//100�t���[����������
	if (Timer % 100 == 0)
	{
		//�O�_�b�V�����Ă݂�
		OdaMoveEnum = ECPPOdaEnum::Moveflont;

		//�؂�ւ���ɂ������ĕϐ�������������
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
/*
 * �֐����@�@�@�@�FOdaUlt()
 * �������e�@�@�@�F�K�E�Z�U�������邽�߂̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::OdaUlt(int Timer)
{

	if (Timer % 30 == 0 && isUltShotTiming != true)
	{
		isUltShotTiming = true;
	}
	//�������킹�ɍs��
	if (isUltShotTiming != true)
	{
		if (GetActorLocation().Y > UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
		{
			BossMove(OdaSpeed * 2, FVector(0.f, 1.f, 0.f));
		}
		else
		{
			BossMove(OdaSpeed * 2, FVector(0.f, -1.f, 0.f));
		}
	}

	//���[�V����(�A�j���[�V����)���N��������
	else if (isMotionPlaying == true)
	{
		//�A�j���[�V�����𗬂�(���͉�)
		PlayAnimMontage(AnimMontage_BossUltimate);
		//��x�������������̂Ńt���O��؂�ւ���
		isMotionPlaying = false;
	}

	//�Ռ��g���o�������̂�Notify���g���Đ��䂷��(UltSpawnFlagChange�ɂĕϐ��̒��g��ύX)
	if (isShockWaveSpawnTiming == true)
	{
		//�_���[�W�l����
		BladeDamage = UltShockWaveDamage;
		//�A�N�^�[�̃X�|�[������(ShockWaveSpawn�̓u���[�v�����g��Őݒ�)
		GetWorld()->SpawnActor<AActor>(ShockWaveSpawn, GetActorTransform());
		//��x�����X�|�[�����������̂Ő؂�ւ��Ă���
		isShockWaveSpawnTiming = false;
	}

	//�A�j���[�V�����̍Ō�ɕK�E�Z���o�������̂�Notify���g���Đ��䂷��(UltSpawnFlagChange�ɂĕϐ��̒��g��ύX)
	if (isUltSpawnTiming == true)
	{
		//�A�N�^�[�̃X�|�[������(UltSpawn�̓u���[�v�����g��Őݒ�)
		GetWorld()->SpawnActor<AActor>(UltSpawn, GetActorTransform());
		//��x�����X�|�[�����������̂Ő؂�ւ��Ă���
		isUltSpawnTiming = false;
	}



	//300�t���[����������
	if (Timer % 300 == 0)
	{
		//�X�e�[�g��؂�ւ���
		OdaMoveEnum = ECPPOdaEnum::Moveflont;
		//�؂�ւ���ɂ������ĕϐ�������������
		WaitTime = 0;
		isUltShotTiming = false;
	}
}
/*
 * �֐����@�@�@�@�FAttackFlagChange()
 * �������e�@�@�@�FAnimNotify�ɂĕύX�A�U���̃t���O��on,off�ɂ��Ă̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::AttackFlagChange()
{
	//���]������
	IsAttackNow = !IsAttackNow;
}


/*
 * �֐����@�@�@�@�FShockWaveSpawnFlagChange()
 * �������e�@�@�@�F�Ռ��g���X�|�[�������邽�߂̕ϐ���؂�ւ���ׂ̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::ShockWaveSpawnFlagChange()
{
	//�Ռ��g���o��������ׂ̕ϐ���true�ɕς���
	isShockWaveSpawnTiming = true;
	//����IsAttackNow��true�ɂȂ��Ă�����
	if (IsAttackNow == true)
	{
		//false�ɂ���
		IsAttackNow = false;
	}
}

/*
 * �֐����@�@�@�@�FUltSpawnFlagChange()
 * �������e�@�@�@�F�K�E�Z���X�|�[�������邽�߂̕ϐ���؂�ւ���ׂ̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
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

/*
 * �֐����@�@�@�@�FApplyDamage()
 * �������e�@�@�@�F�_���[�W���󂯂����̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::ApplyDamage(float Damage)
{
	//�������~�߂�
	OdaMoveEnum = ECPPOdaEnum::Stop;

	if (isHPLock != true)
	{
		//�����񕪃_���[�W���󂯂��甽�������鏈��
		if (NotAttackCount < 5)
		{
			//�^�C�}�[�����Z�b�g
			WaitTime = 0;
			NotAttackCount++;
		}
		else
		{
			//�����ɓ���Ƃ��̓{�X���������鏈���Ȃ̂ł��ꑊ���̑��x��Ԃ�
			Attack1Delay = 1;
		}
		//�{�X�̃X�|�[�����ҋ@���Ԓ��ɍU�����󂯂���
		if (SpawnDelay == true)
		{
			//�t���O��؂�ւ���
			SpawnDelay = false;
			//�U���̃f�B���C���Z�b�g
			Attack1Delay = 10;
		}
		//�G�t�F�N�g�̓v���C���[�̕��ł���Ă���Ă���
		if (isMove)
		{
			Health -= Damage;
			if (isBossHPRock)
			{
				if (FMath::RandRange(0, 100) <= 15)
				{
					//�m�b�N�o�b�N�A�j���[�V�����𗬂�
					PlayAnimMontage(AnimMontage_BossBlowAway);

					//�w�ʃ_�b�V���ɐ؂�ւ��Ďd�؂蒼��
					OdaMoveEnum = ECPPOdaEnum::MoveBack;

					//�A�j���[�V�����I����HP���b�N����������Stay�ɖ߂�
					FTimerManager& TimerManager = GetWorld()->GetTimerManager();
					TimerManager.SetTimer(TimerHandle_KnockbackAnimationFinish, this, &AOdaBase::BossHPRock, AnimMontage_BossBlowAway->GetPlayLength() - 1.1f, false);
					TimerManager.SetTimer(TimerHandle_KnockbackAnimationFinish, this, &AOdaBase::BacktoStayCase, AnimMontage_BossBlowAway->GetPlayLength(), false);


				}
				else
				{
					//�m�b�N�o�b�N�̃A�j���[�V�����𗬂�
					PlayAnimMontage(AnimMontage_BossKnockMontage);
					// �A�j���[�V�����I����Stay�ɖ߂�
					FTimerManager & TimerManager = GetWorld()->GetTimerManager();
					TimerManager.SetTimer(TimerHandle_KnockbackAnimationFinish, this, &AOdaBase::BossHPRock, AnimMontage_BossKnockMontage->GetPlayLength() - 1.1f, false);
					TimerManager.SetTimer(TimerHandle_KnockbackAnimationFinish, this, &AOdaBase::BacktoStayCase, AnimMontage_BossKnockMontage->GetPlayLength(), false);

				}
			}
		}

		//HP��0�ɂȂ�����
		if (Health <= 0.f)
		{
			//�����邩�ǂ����̕ϐ���false�ɂ��Ă���
			if (isMove == true)
			{
				//�t���O���g���ă_���[�W���󂯂�A�j���[�V�����𓮂����Ȃ��悤�ɂ���
				isMove = false;
				//�ꉞ�A�j���[�V�������~�߂Ă���
				this->StopAnimMontage();

				//�Q�[���S�̂̑��x��x������
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), .2f);

				//�A�j���[�V�����𗬂�(���͉�)
				PlayAnimMontage(AnimMontage_BossDeath);

				//�A�j���[�V�����̒����𔻒肵�Ă��̎��Ԃ��߂����玀�S�����̊֐��ɔ�΂��鏈��
				FTimerManager& TimerManager2 = GetWorld()->GetTimerManager();
				TimerManager2.SetTimer(TimerHandle_DeathToGameOver, this, &AOdaBase::Death, AnimMontage_BossDeath->GetPlayLength() - 0.5f, false);

				//��x�������������̂Ńt���O��؂�ւ���
				isMotionPlaying = false;
			}
		}
	}
}
/*
 * �֐����@�@�@�@�FWorldTimeReturn()
 * �������e�@�@�@�F�����ƒx���܂܂��ƃX�g���X�����܂�̂Ō��̑��x�ɖ߂�(�A�j���[�V�����ʒm�ŊǗ�)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::WorldTimeReturn()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

/*
 * �֐����@�@�@�@�FBossHPRock()
 * �������e�@�@�@�F�{�X��HP�����炳�Ȃ��悤�ɂ��鏈��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::BossHPRock()
{
	isBossHPRock = !isBossHPRock;
}

/*
 * �֐����@�@�@�@�FBacktoStayCase()
 * �������e�@�@�@�F�{�X�̍s����ҋ@�ɖ߂�����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::BacktoStayCase()
{
	//�ҋ@�ɖ߂�
	OdaMoveEnum = ECPPOdaEnum::Stay1;
}


/*
 * �֐����@�@�@�@�FBossKnockback()
 * �������e�@�@�@�F�{�X���m�b�N�o�b�N���鏈��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::BossKnockback()
{
	//���S������Ȃ�������
	if (!isBossHPRock)
	{
		//�{�X���m�b�N�o�b�N���鏈��
		PlayAnimMontage(AnimMontage_BossBlowAway);
	}
}

/*
 * �֐����@�@�@�@�FHPLock()
 * �������e�@�@�@�FHP���b�N�p�̃X�C�b�`��؂�ւ��鏈��(�v���C���[��)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::HPLock()
{
	//HP���b�N�p�̃X�C�b�`��؂�ւ���
	isHPLock = !isHPLock;
}

//�����֘A

/*
 * �֐����@�@�@�@�FBossMove()
 * �������e�@�@�@�F���ʕ����ɕ�������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::BossMove(float Speed, FVector MoveSize)
{
	//���ʕ����ɕ�������
	AddMovementInput(MoveSize, 1.f);
}

/*
 * �֐����@�@�@�@�FGetPlayer()
 * �������e�@�@�@�F�v���C���[���擾���鏈��
 * �߂�l�@�@�@�@�FAActor(�v���C���[�̏��)
 */
AActor* AOdaBase::GetPlayer()
{
	PlayerChara = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	return PlayerChara;
}

/*
 * �֐����@�@�@�@�FtoPlayerAttacked()
 * �������e�@�@�@�F�v���C���[�ɑ΂��Ă�HP���b�N���邽�߂̊֐�
 * �߂�l�@�@�@�@�FAActor(�v���C���[�̏��)
 */
void AOdaBase::toPlayerAttacked()
{
	if (bIsAttacked)
	{
		bIsAttacked = false;
	}
}

/*
 * �֐����@�@�@�@�FIs1Combo()
 * �������e�@�@�@�F�ߋ����U���̃R���{�����邽�߂̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::Is1Combo()
{
	if (!bIsAttacked || Combo1Counter >= 3)
	{
		//�A�j���[�V�������~�߂�
		GetMesh()->GetAnimInstance()->StopSlotAnimation();
		//�ϐ������Z�b�g
		isAttack1Waiting = true;
		Combo1Counter = 0;
		HoldDamageAdd = 0;
	}
	else
	{
		Combo1Counter++;
	}
}
/*
 * �֐����@�@�@�@�FIs2Combo()
 * �������e�@�@�@�F�������U���̃R���{�����邽�߂̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::Is2Combo()
{
	if (Combo2Counter == 0)
	{
		//HP��2/3�ȉ�����Ȃ�������ȉ��̏���
		if (Health > MaxHealth * (2.f / 3.f))
		{
			//�A�j���[�V�������~�߂�
			GetMesh()->GetAnimInstance()->StopSlotAnimation();
			//�ϐ������Z�b�g
			Combo2Counter = 0;

		}
		else
		{
			Combo2Counter++;
		}
	}
	else if (Combo2Counter == 1)
	{
		//HP��1/3�ȉ�����Ȃ�������ȉ��̏���
		if (Health > MaxHealth * (1.f / 3.f))
		{
			//�A�j���[�V�������~�߂�
			GetMesh()->GetAnimInstance()->StopSlotAnimation();
			//�ϐ������Z�b�g
			Combo2Counter = 0;
		}
		else
		{
			Combo2Counter++;
		}
	}
	else if (Combo2Counter)
	{
		//�ϐ������Z�b�g
		Combo2Counter = 0;
	}
	//UKismetSystemLibrary::PrintString(this, LastAttack() ? TEXT("true") : TEXT("false"), true, true, FColor::Cyan, 2.f, TEXT("None"));
}
/*
 * �֐����@�@�@�@�FLastAttack()
 * �������e�@�@�@�F�v���C���[���m�b�N�o�b�N�����邽�߂ɍŌ�̍U�����ǂ������Ƃ�֐�
 * �߂�l�@�@�@�@�Fbool(�Ō�̍U���Ȃ�true��������)
 */
bool AOdaBase::LastAttack()
{
	if (isMove)
	{
		if (Combo1Counter >= 3)
		{
			return true;
		}

		else if (Combo2Counter == 2)
		{
			return true;
		}
	}
	return false;
}

//����Death���ĂԂ��߂̊֐�
void AOdaBase::DeathMotion()
{
	Death();
}

/*
 * �֐����@�@�@�@�FDeath()
 * �������e�@�@�@�F�̗͂�0�ȉ��ɂȂ����Ƃ��ɓ����֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::Death()
{
	//�Q�[�����[�h�ɂĂ��̃A�N�^����������
	ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		if (this != NULL)
		{
			//���݃G���[�ŗ�����̂ŕ��ʂɏ���
			//�Q�[�����[�h��p���ď���
			GameMode->DestroyEnemy(this, IsAreaEnemy);
			GameMode->SetState_GameClear(0);
		}
	}
}

/*
 * �֐����@�@�@�@�FCheckOverlap()
 * �������e�@�@�@�F�{�b�N�X�R���W�����ɓ������Ă��邩�`�F�b�N
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::CheckOverlap()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	FVector Start = BoxComp->GetComponentLocation();
	FVector End = Start;// + GetActorForwardVector() * 100.0f;
	FQuat Rot = BoxComp->GetComponentQuat();			// 
	FCollisionShape CollisionBox = FCollisionShape::MakeBox(BoxComp->GetUnscaledBoxExtent());

	bool isHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, Rot, ECollisionChannel::ECC_GameTraceChannel1, CollisionBox, CollisionParams);

	if (isHit != true) { return; }

	if (false) {
		DrawDebugCapsule(GetWorld(), (Start + End) / 2,
			CollisionBox.GetCapsuleHalfHeight(),
			CollisionBox.GetCapsuleRadius(), Rot, FColor::Red, true, -1.0f, 0, 1.0f);
	}

	for (FHitResult HitResult : HitResults) {
		PlayerOnOverlap(HitResult);
	}
}

/*
 * �֐����@�@�@�@�FCheckOverlap()
 * �������e�@�@�@�F�v���C���[������������̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::PlayerOnOverlap(FHitResult& _HitResult)
{
	//Cast
	APlayerBase* Player = Cast<APlayerBase>(_HitResult.GetActor());
	if (Player) {
		//�v���C���[��HP�����b�N�����炱�̏�����ʂ�
		if (bIsAttacked) {
			return;
		}

		if (Combo1Counter == 0)
		{
			Player->TakedDamage(SwordFirstDamage, LastAttack());						//�v���C���[�Ƀ_���[�W��^���鏉�i
			UKismetSystemLibrary::PrintString(this, LastAttack() ? TEXT("true") : TEXT("false"), true, true, FColor::Cyan, 2.f, TEXT("None"));

		}
		else
		{
			//�ǉ��_���[�W�l�m�F�p
			//UKismetSystemLibrary::PrintString(this, FString::FromInt(SwordFirstDamage + ((float)SwordFirstDamage*(float)(Combo1Counter/4.f))), true, true, FColor::Cyan, 2.f, TEXT("None"));

			Player->TakedDamage(SwordFirstDamage + ((float)SwordFirstDamage * (float)(Combo1Counter / 4.f)), LastAttack());						//�v���C���[�Ƀ_���[�W��^����(�㔼�̏����̓R���{���̒ǉ��_���[�W)
			UKismetSystemLibrary::PrintString(this, LastAttack() ? TEXT("true") : TEXT("false"), true, true, FColor::Cyan, 2.f, TEXT("None"));
		}
		//�q�b�g�X�g�b�v��������
		ActionAssistComp->HitStop(1.f, .2f);

		//���Z�b�g
		bIsAttacked = true;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "OdaBase.h"
#include "GameSystem/TGS_GameMode.h"
#include "GameSystem/EnemyBase_WidgetComponent.h"



// Sets default values
//�R���X�g���N�^+�ϐ��̏�����
AOdaBase::AOdaBase() :
	SpawnTimer(0), 
	FlameCounter(0),
	WaitTime(0),
	OdaMoveEnum(ECPPOdaEnum::Stay1),
	OdaSpeed(1.f),
	Attack1Delay(0),
	RandomNum(-1),
	OneMoreShockWave(false),
	ChangeFlontTimer(20),
	isMotionPlaying(false),
	isShockWaveSpawnTiming(false),
	isUltShotTiming(false),
	isUltShot(true),
	UltTimer(550),
	isNotAttackNow(false),
	NotAttackCount(0),
	SwordDamage(10),
	bIsAttacked(false),
	Health(600.f),
	MaxHealth(600.f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordComponent"), true);
	BoxComponent->SetupAttachment(GetMesh(), "weapon_r");
	BoxComponent->SetRelativeLocation(FVector(0.0f, -80.0f, 0.0f));

	//UI Create
	EnemyWidget = CreateDefaultSubobject<UEnemyBase_WidgetComponent>(TEXT("EnemyWidget"));
	EnemyWidget->SetupAttachment(RootComponent);

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


	if (SpawnDelay != true)
	{
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

		AActor* Player = GetPlayer();
		//���X�|�[���������ɒ��g���Ȃ��Ȃ��Ă��܂��̂ōX�V
		if (Player == nullptr)  return;

		//���������
		//X��
		BossPosX = FVector(GetActorLocation().X, 0.f, 0.f);
		PlayerPosX = FVector(Player->GetActorLocation().X, 0.f, 0.f);

		//Y��
		BossPosY = FVector(0.f, GetActorLocation().Y, 0.f);
		PlayerPosY = FVector(0.f, Player->GetActorLocation().Y, 0.f);

		//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(FVector::Dist(BossPosY, PlayerPosY)), true, true, FColor::Cyan, 2.f, TEXT("None"));

		//�J�E���^�[�N��
		WaitTime++;

		if (Health < MaxHealth / 2.f)
		{
			UltTimer++;
			if (UltTimer % 600 == 0)//600�t���[����ɕK�E�����Ă�悤�ɂ���
			{
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

			//�U���P
		case ECPPOdaEnum::Attack1:
			OdaAttack1(WaitTime);
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
	else//�{�X���X�|�[����������̏���
	{
		//�J�E���^�[���N��
		SpawnTimer++;
		//�J�E���^�[����萔�𒴂�����
		if (SpawnTimer % 300 == 0)
		{
			//�����o�����߂̃t���O��؂�ւ���
			SpawnDelay = false;
		}
	}
}
//Y�����������Ăǂ������ɂ��邩
void AOdaBase::ToPlayerRotate()
{
	bool LookRight;

	//�{�X���v���C���[��葽���Ƃ�
	if (GetActorLocation().Y > UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
	{
		LookRight = true;
		
		//y��270�Ɍ�����(��)
		//SetActorRotation(FRotator(0.f, 270.f, 0.f));
	}
	else
	{
		LookRight = false;

		//y��90�Ɍ�����(�E)
		//SetActorRotation(FRotator(0.f, 90.f, 0.f));
	}

	ActionAssistComp->OwnerParallelToCamera(LookRight);
}

//�ҋ@�֐�
void AOdaBase::OdaStay1(int Timer)
{
	//UKismetSystemLibrary::PrintString(this, "Stay", true, true, FColor::Cyan, 2.f, TEXT("None"));

	//���[�V�����𗬂���Ԃ��ǂ���(isMotionPlaying��true�ɂ���)
	if (isMotionPlaying != true)
	{
		isMotionPlaying = true;
	}
	//�U���I������----------------------------------------------------------------------------------------------------------------------------------------------

	////������x�������U�����I�����ꂽ��
	//if (OneMoreShockWave)
	//{
	//	//�������U��
	//	OdaMoveEnum = ECPPOdaEnum::Attack2;
	//	WaitTime = 0;
	//	//random�Œ�̃��Z�b�g
	//	if (RandomNum != -1)
	//	{
	//		RandomNum = -1;
	//	}

	//}

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
				Attack1Delay = 20;
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
			Attack1Delay = 30;
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


void AOdaBase::OdaMove1(int DeltaTime, int StopTimer)
{
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


//�U���P
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
		OdaMoveEnum = ECPPOdaEnum::Stay1;
		//�؂�ւ���ɂ������ĕϐ�������������
		WaitTime = 0;
		//���Z�b�g
		NotAttackCount = 0;
		//�m�b�N�o�b�N���ɍU�����[�V�����ɓ����HP���b�N���쓮�������Ă��܂��̂ł����Ő؂�ւ���(���}���u)
		if (isHPLock == true)
		{
			HPLock();
		}
	}
}

//�U���Q
void AOdaBase::OdaAttack2(int Timer) {
	//UKismetSystemLibrary::PrintString(this, "Attack2", true, true, FColor::Cyan, 2.f, TEXT("None"));
	
	//���[�V����(�A�j���[�V����)���N��������
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
		GetWorld()->SpawnActor<AActor>(ShockWaveSpawn,GetActorTransform());
		//��x�����X�|�[�����������̂Ő؂�ւ��Ă���
		isShockWaveSpawnTiming = false;
	}

	//150�t���[����������
	if (Timer % 150 == 0)
	{
		////��������false�ɂ���
		//OneMoreShockWave = false;
		////�X�e�[�g��؂�ւ���(�E�̐������̊m���ł�����x�Ռ��g���΂��Ă݂�)
		//if (FMath::RandRange(0, 100) <= 80)
		//{
		//	UKismetSystemLibrary::PrintString(this, "OneMore", true, true, FColor::Cyan, 2.f, TEXT("None"));
		//	//������x�ł��߂̕ϐ���true������
		//	OneMoreShockWave = true;
		//}
		//else
		//{

			OdaMoveEnum = ECPPOdaEnum::Stay1;

		//}
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
void AOdaBase::OdaUlt(int Timer)
{
	//UKismetSystemLibrary::PrintString(this, "Ult", true, true, FColor::Cyan, 2.f, TEXT("None"));
	if (Timer % 50 == 0 && isUltShotTiming != true)
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
	//�A�j���[�V�����̍Ō�ɕK�E�Z���o�������̂�Notify���g���Đ��䂷��(UltSpawnFlagChange�ɂĕϐ��̒��g��ύX)

	if (isUltSpawnTiming == true)
	{
		//�A�N�^�[�̃X�|�[������(UltSpawn�̓u���[�v�����g��Őݒ�)
		GetWorld()->SpawnActor<AActor>(UltSpawn, GetActorTransform());
		//��x�����X�|�[�����������̂Ő؂�ւ��Ă���
		isUltSpawnTiming = false;
	}



	//200�t���[����������
	if (Timer % 200 == 0)
	{
		//�X�e�[�g��؂�ւ���
		OdaMoveEnum = ECPPOdaEnum::Stay1;
		//�؂�ւ���ɂ������ĕϐ�������������
		WaitTime = 0;
		isUltShotTiming = false;
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

//�_���[�W���󂯂����̏���
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
			Attack1Delay = 30;
		}
		//�G�t�F�N�g���o��
		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticles, GetActorLocation());
		ActionAssistComp->SpawnHitEffect(HitParticles, GetActorLocation());

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

void AOdaBase::BossKnockback()
{
	PlayAnimMontage(AnimMontage_BossBlowAway);
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
	AddMovementInput(MoveSize, 1.f);
}

//�v���C���[���擾���鏈��
AActor* AOdaBase::GetPlayer()
{
	PlayerChara = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	return PlayerChara;
}

//�v���C���[�ɑ΂��Ă�HP���b�N���邽�߂̊֐�
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
		PlayerOnOverlap(HitResult);
	}
}
//�v���C���[������������
void AOdaBase::PlayerOnOverlap(FHitResult& _HitResult)
{
	//Cast
	APlayerBase* Player = Cast<APlayerBase>(_HitResult.GetActor());
	if (Player) {
		//�v���C���[��HP�����b�N�����炱�̏�����ʂ�
		if (bIsAttacked) {
			return;
		}

		Player->TakedDamage(SwordDamage);						//�v���C���[�Ƀ_���[�W��^����
		ActionAssistComp->HitStop(.2f);

		//���Z�b�g
		bIsAttacked = true;
	}
}

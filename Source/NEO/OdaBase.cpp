// Fill out your copyright notice in the Description page of Project Settings.


#include "OdaBase.h"
#include "GameSystem/TGS_GameMode.h"
#include "GameSystem/EnemyBase_WidgetComponent.h"



// Sets default values
//コンストラクタ+変数の初期化
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

	//フレームごとに加算する
	FlameCounter++;
	//向きをプレイヤーの方に向ける(60フレーム毎に更新)
	if (FlameCounter % 60 == 0)
	{
		//プレイヤーの方を向く
		ToPlayerRotate();
	}
	//ボスの前情報を取る
	ForwardDirection = GetActorForwardVector();
	//カウンター起動
	WaitTime++;
	//UKismetSystemLibrary::PrintString(this, FString::FromInt(WaitTime), true, true, FColor::Cyan, 2.f, TEXT("None"));

	

		//状態ごとに動きを切り替える
		switch (OdaMoveEnum)
		{
			//待機
		case ECPPOdaEnum::Stay1:
			OdaStay1(WaitTime);
			break;

			//動き１
		case ECPPOdaEnum::Move1:
			OdaMove1(WaitTime);
			break;

			//動き２
		case ECPPOdaEnum::Move2:
			OdaMove2(WaitTime);
			break;		

			//退却
		case ECPPOdaEnum::Back1:
			OdaBack1(WaitTime);
			break;

			//攻撃１
		case ECPPOdaEnum::Attack1:
			OdaAttack1(WaitTime);
			break;

			//攻撃２
		case ECPPOdaEnum::Attack2:
			OdaAttack2(WaitTime);
		default:

			break;
		}

}
//Y軸だけを見てどっち側にいるか
void AOdaBase::ToPlayerRotate()
{
	//ボスがプレイヤーより多いとき
	if (this->GetActorLocation().Y > UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
	{
		//yを270に向ける(左)
		this->SetActorRotation(FRotator(0.f, 270.f, 0.f));
	}
	else
	{
		this->SetActorRotation(FRotator(0.f, 90.f, 0.f));
	}
}
void AOdaBase::OdaStay1(int Timer)
{
	//モーションを流す状態かどうか(isMotionPlayingをtrueにする)
	if (isMotionPlaying != true)
	{
		isMotionPlaying = true;
	}
	//プレイヤーとの距離が400離れるか60フレームたったら
	if (GetDistanceTo(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) >= 400 && WaitTime >= 60)
	{
		//二分一
		
		if (FMath::RandRange(0, 1) == 0)
		{
			//動き１へ
			OdaMoveEnum = ECPPOdaEnum::Move1;
			SwitchStayMove = false;
		}
		else
		{
			//動き２へ
			OdaMoveEnum = ECPPOdaEnum::Move2;
			SwitchStayMove = false;
		}
	}
}

void AOdaBase::OdaMove1(int Timer)
{
	//2秒たつかプレイヤーとY軸が同じになったら切り替える
	if (Timer % 120 == 0 || FMath::IsNearlyEqual
	(this->GetActorLocation().Y,UGameplayStatics::GetPlayerPawn(GetWorld(),0)->GetActorLocation().Y,400.f))
	{
		//反転
		SwitchStayMove = true;
	}
	//詰める
	if (SwitchStayMove)
	{
		//動かす処理を呼び出す
		BossMove(FastOdaSpeed*2, ForwardDirection);
	}
	//軸を合わせる
	else
	{ 
		//どっち側にいるか(前方向に走る)
		if (this->GetActorLocation().X < UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().X)
		{
			BossMove(FastOdaSpeed * 2, FVector(1.f,0.f,0.f));
		}
		else
		{
			BossMove(FastOdaSpeed * 2, FVector(-1.f, 0.f, 0.f));
		}
	}
	//プレイヤーとの距離が近かったら
	if (GetDistanceTo(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) <= 300.f) {
		OdaMoveEnum = ECPPOdaEnum::Attack1;
		SwitchStayMove = false;
		WaitTime = 0;
	}
}

void AOdaBase::OdaMove2(int Timer)
{
	//2秒たつかプレイヤーとY軸が同じになったら切り替える
	if (Timer % 120 == 0 || FMath::IsNearlyEqual
	(this->GetActorLocation().Y, UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y, 400.f))
	{
		//反転
		SwitchStayMove = true;
	}
	//衝撃波を打つ(衝撃波自体はブループリントで)
	if (SwitchStayMove)
	{
		OdaMoveEnum = ECPPOdaEnum::Attack2;
		SwitchStayMove = false;
		WaitTime = 0;
	}
	//軸を合わせる
	else
	{
		//どっち側にいるか
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

//ちょっと後ろに下がるやつ
void AOdaBase::OdaBack1(int Timer) {
	//50フレーム後待機に戻る
	if (Timer % 50 == 0)
	{
		OdaMoveEnum = ECPPOdaEnum::Stay1;
		SwitchStayMove = false;
		WaitTime = 0;
	}
	//後ろに下がる
	BackMove(FastOdaSpeed * 10.f);
}

//攻撃１
void AOdaBase::OdaAttack1(int Timer) {
	if (isMotionPlaying == true)
	{
		//アニメーションを流す(今は仮)
		PlayAnimMontage(AnimMontage_BossAttack1);
		//一度だけ流したいのでフラグを切り替える
		isMotionPlaying = false;
	}

	//150フレームたったら
	if (Timer % 150 == 0)
	{
		//ステートを切り替える
		OdaMoveEnum = ECPPOdaEnum::Stay1;
		//切り替えるにあたって変数を初期化する
		SwitchStayMove = false;
		WaitTime = 0;
	}
}

//攻撃２
void AOdaBase::OdaAttack2(int Timer) {
	if (isMotionPlaying == true)
	{
		//アニメーションを流す(今は仮)
		PlayAnimMontage(AnimMontage_BossAttack2);
		//一度だけ流したいのでフラグを切り替える
		isMotionPlaying = false;
	}


	//アニメーションの最後に衝撃波を出したいのでNotifyを使って制御する(ShockWaveSpawnFlagChangeにて変数の中身を変更)
	if (isShockWaveSpawnTiming == true)
	{
		//アクターのスポーン処理(ShockWaveSpawnはブループリント上で設定)
		GetWorld()->SpawnActor<AActor>(ShockWaveSpawn, GetActorTransform());
		//一度だけスポーンさせたいので切り替えておく
		isShockWaveSpawnTiming = false;
	}

	//150フレームたったら
	if (Timer % 150 == 0)
	{
		//ステートを切り替える
		OdaMoveEnum = ECPPOdaEnum::Stay1;
		//切り替えるにあたって変数を初期化する
		SwitchStayMove = false;
		WaitTime = 0;
	}
}


//AnimNotifyにて変更、攻撃のフラグのon,off
void AOdaBase::AttackFlagChange()
{
	//反転させる
	IsAttackNow = !IsAttackNow;
}

//これが呼ばれたら衝撃波をスポーンさせるための変数を切り替える
void AOdaBase::ShockWaveSpawnFlagChange()
{
	//衝撃波を出現させる為の変数をtrueに変える
	isShockWaveSpawnTiming = true;
	//もしIsAttackNowがtrueになっていたら
	if (IsAttackNow == true)
	{
		//falseにしてみる
		IsAttackNow = false;
	}
}


void AOdaBase::ApplyDamage(float Damage)
{
	if (isHPLock != true)
	{
		//ノックバックのアニメーションを流す
		PlayAnimMontage(AnimMontage_BossKnockMontage);
		Health -= Damage;
		//HPが0になったら
		if (Health <= 0.f)
		{
			//体力が0以下になったときの処理
			Death();
		}
	}
}

void AOdaBase::HPLock()
{
	//HPロック用のスイッチを切り替える
	isHPLock = !isHPLock;
}

//動き関連
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

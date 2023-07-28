// Fill out your copyright notice in the Description page of Project Settings.


#include "OdaBase.h"
#include "GameSystem/TGS_GameMode.h"
#include "GameSystem/EnemyBase_WidgetComponent.h"



// Sets default values
//コンストラクタ+変数の初期化
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

	//フレームごとに加算する
	FlameCounter++;
	//向きをプレイヤーの方に向ける(60フレーム毎に更新)
	if (FlameCounter % 60 == 0)
	{
		//プレイヤーの方を向く
		ToPlayerRotate();
	}

	//距離を取る
	//X軸
	BossPosX = FVector(GetActorLocation().X, 0.f, 0.f);
	PlayerPosX = FVector(PlayerChara->GetActorLocation().Y, 0.f, 0.f);

	//Y軸
	BossPosY = FVector(0.f, GetActorLocation().Y, 0.f);
	PlayerPosY = FVector(0.f, PlayerChara->GetActorLocation().Y, 0.f);

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

			//必殺技
		case ECPPOdaEnum::Ultimate:
			OdaUlt(WaitTime);

			break;
		}

}
//Y軸だけを見てどっち側にいるか
void AOdaBase::ToPlayerRotate()
{
	//ボスがプレイヤーより多いとき
	if (GetActorLocation().Y > UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
	{
		//yを270に向ける(左)
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

	//モーションを流す状態かどうか(isMotionPlayingをtrueにする)
	if (isMotionPlaying != true)
	{
		isMotionPlaying = true;
	}
	//HPが50%以下になったら

	if (Health < MaxHealth/2.f)
	{
		if (isUltShot == false)
		{
			//必殺技
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


	//プレイヤーとの距離の判定
	if (FVector::Dist(BossPosY, PlayerPosY) <= 200.f)
	{
		//近接攻撃
		OdaMoveEnum = ECPPOdaEnum::Attack1;
		WaitTime = 0;
	}
	if (FVector::Dist(BossPosX, PlayerPosX) <= 50.f)
	{

		if(FVector::Dist(BossPosY, PlayerPosY) >= 300.f)
		{
			//遠距離
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
				//軸を合わせに行く
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

//ちょっと後ろに下がるやつ
void AOdaBase::OdaBack1(int Timer) {
	UKismetSystemLibrary::PrintString(this, "Back", true, true, FColor::Cyan, 2.f, TEXT("None"));
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
	UKismetSystemLibrary::PrintString(this, "Attack1", true, true, FColor::Cyan, 2.f, TEXT("None"));


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
		//リセット
		NotAttackCount = 0;
		//ノックバック中に攻撃モーションに入るとHPロックが作動し続けてしまうのでここで切り替える
		if (isHPLock == true)
		{
			HPLock();
		}
	}
}

//攻撃２
void AOdaBase::OdaAttack2(int Timer) {
	UKismetSystemLibrary::PrintString(this, "Attack2", true, true, FColor::Cyan, 2.f, TEXT("None"));
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
		//リセット
		NotAttackCount = 0;
		//ノックバック中に攻撃モーションに入るとHPロックが作動し続けてしまうのでここで切り替える
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
			//アニメーションを流す(今は仮)
			PlayAnimMontage(AnimMontage_BossUltimate);
			//一度だけ流したいのでフラグを切り替える
			isMotionPlaying = false;
		}
	//アニメーションの最後に必殺技を出したいのでNotifyを使って制御する(UltSpawnFlagChangeにて変数の中身を変更)

		if (isUltSpawnTiming == true)
		{
			//アクターのスポーン処理(UltSpawnはブループリント上で設定)
			GetWorld()->SpawnActor<AActor>(UltSpawn, GetActorTransform());
			//一度だけスポーンさせたいので切り替えておく
			isUltSpawnTiming = false;
		}
	}


	//200フレームたったら
	if (Timer % 200 == 0)
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

//これが呼ばれたら必殺技をスポーンさせるための変数を切り替える
void AOdaBase::UltSpawnFlagChange()
{
	//必殺技を出現させる為の変数をtrueに変える
	isUltSpawnTiming = true;
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
		if (NotAttackCount < 5)
		{
			//タイマーをリセット
			WaitTime = 0;
			NotAttackCount++;
		}

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
	//(仮)正面方向に歩く処理
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
	//ゲームモードにてこのアクタを消す処理
	ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->DestroyEnemy(this, IsAreaEnemy);
	}
}

//チェック
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
		//プレイヤーがHPをロックしたらこの処理を通る
		if (bIsAttacked) {
			return;
		}

		Player->TakedDamage(SwordDamage);						//プレイヤーにダメージを与える

		//リセット
		bIsAttacked = true;

		FVector HitLocation = _HitResult.Location;		//ヒットエフェクトの位置
	}
}

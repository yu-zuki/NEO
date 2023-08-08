// Fill out your copyright notice in the Description page of Project Settings.


#include "OdaBase.h"
#include "GameSystem/TGS_GameMode.h"
#include "GameSystem/EnemyBase_WidgetComponent.h"



// Sets default values
//コンストラクタ+変数の初期化
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

	//ディレイを一定時間か攻撃を受けたら処理を切り替える
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

		//フレームごとに加算する
		FlameCounter++;
		//向きをプレイヤーの方に向ける(60フレーム毎に更新)
		if (FlameCounter % 60 == 0)
		{
			//プレイヤーの方を向く
			ToPlayerRotate();
		}

		AActor* Player = GetPlayer();
		//リスポーンした時に中身がなくなってしまうので更新
		if (Player == nullptr)  return;

		//距離を取る
		//X軸
		BossPosX = FVector(GetActorLocation().X, 0.f, 0.f);
		PlayerPosX = FVector(Player->GetActorLocation().X, 0.f, 0.f);

		//Y軸
		BossPosY = FVector(0.f, GetActorLocation().Y, 0.f);
		PlayerPosY = FVector(0.f, Player->GetActorLocation().Y, 0.f);

		//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(FVector::Dist(BossPosY, PlayerPosY)), true, true, FColor::Cyan, 2.f, TEXT("None"));

		//カウンター起動
		WaitTime++;

		if (Health < MaxHealth / 2.f)
		{
			UltTimer++;
			if (UltTimer % 600 == 0)//600フレーム後に必殺を撃てるようにする
			{
				isUltShot = false;
			}
		}

		//状態ごとに動きを切り替える
		switch (OdaMoveEnum)
		{
			//待機,動き
		case ECPPOdaEnum::Stay1:
			OdaStay1(WaitTime);
			break;

			//攻撃１
		case ECPPOdaEnum::Attack1:
			OdaAttack1(WaitTime);
			break;

			//攻撃２
		case ECPPOdaEnum::Attack2:
			OdaAttack2(WaitTime);
			break;

			//必殺技
		case ECPPOdaEnum::Ultimate:
			OdaUlt(WaitTime);
			break;

		default:
			break;
		}

	}
	else//ボスがスポーンした直後の処理
	{
		//カウンターを起動
		SpawnTimer++;
		//カウンターが一定数を超えたら
		if (SpawnTimer % 300 == 0)
		{
			//動き出すためのフラグを切り替える
			SpawnDelay = false;
		}
	}
}
//Y軸だけを見てどっち側にいるか
void AOdaBase::ToPlayerRotate()
{
	bool LookRight;

	//ボスがプレイヤーより多いとき
	if (GetActorLocation().Y > UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
	{
		LookRight = true;
		
		//yを270に向ける(左)
		//SetActorRotation(FRotator(0.f, 270.f, 0.f));
	}
	else
	{
		LookRight = false;

		//yを90に向ける(右)
		//SetActorRotation(FRotator(0.f, 90.f, 0.f));
	}

	ActionAssistComp->OwnerParallelToCamera(LookRight);
}

//待機関数
void AOdaBase::OdaStay1(int Timer)
{
	//UKismetSystemLibrary::PrintString(this, "Stay", true, true, FColor::Cyan, 2.f, TEXT("None"));

	//モーションを流す状態かどうか(isMotionPlayingをtrueにする)
	if (isMotionPlaying != true)
	{
		isMotionPlaying = true;
	}
	//攻撃選択処理----------------------------------------------------------------------------------------------------------------------------------------------

	////もう一度遠距離攻撃が選択されたら
	//if (OneMoreShockWave)
	//{
	//	//遠距離攻撃
	//	OdaMoveEnum = ECPPOdaEnum::Attack2;
	//	WaitTime = 0;
	//	//random固定のリセット
	//	if (RandomNum != -1)
	//	{
	//		RandomNum = -1;
	//	}

	//}

	else if (FVector::Dist(BossPosX, PlayerPosX) <= 50.f)
	{

		if (isUltShot == false)
		{
			//必殺技
			OdaMoveEnum = ECPPOdaEnum::Ultimate;
			isUltShot = true;
			UltTimer = 0;

			//random固定のリセット
			if (RandomNum != -1)
			{
				RandomNum = -1;
			}
		}
		//プレイヤーとの距離の判定
		else if (FVector::Dist(BossPosY, PlayerPosY) <= 200.f)
		{
			//近接攻撃
			OdaMoveEnum = ECPPOdaEnum::Attack1;
			WaitTime = 0;
			//もし攻撃のディレイの値が入っていなかったら
			if (Attack1Delay != 0)
			{
				Attack1Delay = 20;
			}

			//random固定のリセット
			if (RandomNum != -1)
			{
				RandomNum = -1;
			}
		}
		//離れていたら
		else if (FVector::Dist(BossPosY, PlayerPosY) >= 300.f)
		{
			//遠距離
			OdaMoveEnum = ECPPOdaEnum::Attack2;
			WaitTime = 0;
			//random固定のリセット
			if (RandomNum != -1)
			{
				RandomNum = -1;
			}
		}
		else
		{
			//軸を合わせに行く
			OdaMove1(Timer, 60);
			Attack1Delay = 30;
		}
		//-----------------------------------------------------------------------------------------------------------------------------------------------

	}
	else
	{
		//軸を合わせに行く
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
	//軸を合わせに行く
	//randomの中に値が入っているか(-1が入っていない状態)
	if (RandomNum == -1)
	{
		//randomで0か1を入れる
		RandomNum = FMath::RandRange(0, 1);
	}

	//前か後ろに動く処理
	if (RandomNum == 0)
	{
		BossMove(OdaSpeed * 2, FVector(0.f, 1.f, 0.f));
	}
	else if (RandomNum == 1)
	{
		BossMove(OdaSpeed * 2, FVector(0.f, -1.f, 0.f));
	}

}


//攻撃１
void AOdaBase::OdaAttack1(int Timer) {
	//UKismetSystemLibrary::PrintString(this, "Attack1", true, true, FColor::Cyan, 2.f, TEXT("None"));
	if (Attack1Delay == 0)
	{
		//0だと割り切れないので1を代入しておく
		Attack1Delay = 1;
	}

	//設定した時間を超えたか
	if (Timer % Attack1Delay == 0)
	{
		//モーション(アニメーション)が起動したか
		if (isMotionPlaying == true)
		{
			//アニメーションを流す(今は仮)
			PlayAnimMontage(AnimMontage_BossAttack1);
			//一度だけ流したいのでフラグを切り替える
			isMotionPlaying = false;
			//ディレイをリセットする
			Attack1Delay = 0;
		}
	}

	//150フレームたったら
	if (Timer % 150 == 0)
	{
		//ステートを切り替える
		OdaMoveEnum = ECPPOdaEnum::Stay1;
		//切り替えるにあたって変数を初期化する
		WaitTime = 0;
		//リセット
		NotAttackCount = 0;
		//ノックバック中に攻撃モーションに入るとHPロックが作動し続けてしまうのでここで切り替える(応急処置)
		if (isHPLock == true)
		{
			HPLock();
		}
	}
}

//攻撃２
void AOdaBase::OdaAttack2(int Timer) {
	//UKismetSystemLibrary::PrintString(this, "Attack2", true, true, FColor::Cyan, 2.f, TEXT("None"));
	
	//モーション(アニメーション)が起動したか
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
		GetWorld()->SpawnActor<AActor>(ShockWaveSpawn,GetActorTransform());
		//一度だけスポーンさせたいので切り替えておく
		isShockWaveSpawnTiming = false;
	}

	//150フレームたったら
	if (Timer % 150 == 0)
	{
		////いったんfalseにする
		//OneMoreShockWave = false;
		////ステートを切り替える(右の数字分の確率でもう一度衝撃波を飛ばしてみる)
		//if (FMath::RandRange(0, 100) <= 80)
		//{
		//	UKismetSystemLibrary::PrintString(this, "OneMore", true, true, FColor::Cyan, 2.f, TEXT("None"));
		//	//もう一度打つための変数にtrueを入れる
		//	OneMoreShockWave = true;
		//}
		//else
		//{

			OdaMoveEnum = ECPPOdaEnum::Stay1;

		//}
		//切り替えるにあたって変数を初期化する
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
	//UKismetSystemLibrary::PrintString(this, "Ult", true, true, FColor::Cyan, 2.f, TEXT("None"));
	if (Timer % 50 == 0 && isUltShotTiming != true)
	{
		isUltShotTiming = true;
	}
	//軸を合わせに行く
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

	//モーション(アニメーション)が起動したか
	else if (isMotionPlaying == true)
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



	//200フレームたったら
	if (Timer % 200 == 0)
	{
		//ステートを切り替える
		OdaMoveEnum = ECPPOdaEnum::Stay1;
		//切り替えるにあたって変数を初期化する
		WaitTime = 0;
		isUltShotTiming = false;
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

//ダメージを受けた時の処理
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
		else
		{
			//ここに入るときはボスが反撃する処理なのでそれ相応の速度を返す
			Attack1Delay = 1;
		}
		//ボスのスポーン時待機時間中に攻撃を受けた時
		if (SpawnDelay == true)
		{
			//フラグを切り替える
			SpawnDelay = false;
			//攻撃のディレイをセット
			Attack1Delay = 30;
		}
		//エフェクトを出す
		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticles, GetActorLocation());
		ActionAssistComp->SpawnHitEffect(HitParticles, GetActorLocation());

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

void AOdaBase::BossKnockback()
{
	PlayAnimMontage(AnimMontage_BossBlowAway);
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
	AddMovementInput(MoveSize, 1.f);
}

//プレイヤーを取得する処理
AActor* AOdaBase::GetPlayer()
{
	PlayerChara = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	return PlayerChara;
}

//プレイヤーに対してのHPロックするための関数
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
		PlayerOnOverlap(HitResult);
	}
}
//プレイヤーが当たったら
void AOdaBase::PlayerOnOverlap(FHitResult& _HitResult)
{
	//Cast
	APlayerBase* Player = Cast<APlayerBase>(_HitResult.GetActor());
	if (Player) {
		//プレイヤーがHPをロックしたらこの処理を通る
		if (bIsAttacked) {
			return;
		}

		Player->TakedDamage(SwordDamage);						//プレイヤーにダメージを与える
		ActionAssistComp->HitStop(.2f);

		//リセット
		bIsAttacked = true;
	}
}

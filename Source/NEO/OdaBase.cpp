// Fill out your copyright notice in the Description page of Project Settings.


#include "OdaBase.h"

//UAnimNotify 


// Sets default values
//コンストラクタ+変数の初期化
AOdaBase::AOdaBase():
	OdaNobunaga(this),
	NobunagaMovement(OdaNobunaga->GetCharacterMovement()),
	ForwardDirection(OdaNobunaga->GetActorForwardVector()),
	OdaSpeed(1.f),
	FastOdaSpeed(1.65),
	FlontTimer(0),
	OdaMoveEnum(ECPPOdaEnum::Stay1),
	ChangeFlontTimer(20),
	SwitchStayMove(true),
	WaitTime(0),
	Health(500)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AOdaBase::BeginPlay()
{
	Super::BeginPlay();
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
	//向きをプレイヤーの方に向ける
	this->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation()));
	//ボスの前情報を取る
	ForwardDirection = OdaNobunaga->GetActorForwardVector();
	//カウンター起動
		FlontTimer += 1;
		WaitTime += 1;

		//状態ごとに動きを切り替える
		switch (OdaMoveEnum)
		{
			//待機
		case ECPPOdaEnum::Stay1:
			OdaStay1(FlontTimer);
			break;
			//動き１
		case ECPPOdaEnum::Move1:
			OdaMove1(FlontTimer);
			break;
			//動き２
		case ECPPOdaEnum::Move2:
			OdaMove2(FlontTimer);
			break;		
			//退却
		case ECPPOdaEnum::Back1:
			OdaBack1(FlontTimer);
			break;
		default:

			break;
		}

}
void AOdaBase::OdaStay1(int Timer)
{
	if (Timer % 60 == 0)
	{
		//一秒ごとに切り替える
		SwitchStayMove = !SwitchStayMove;
	}
	if (SwitchStayMove)
	{
		//正面方向に移動
		FlontMove(OdaSpeed / 10.f);
	}
	else
	{
		//後方に移動
		BackMove(OdaSpeed / 10.f);
	}

	if (GetDistanceTo(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) >= 400 && WaitTime >= 60)
	{
		//タイマーを÷2したときに余りが0か1か(二分一)
		if (Timer % 2 == 0)
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
	else if (GetDistanceTo(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) <= 300 && WaitTime >= 60)
	{
		//退却へ
		OdaMoveEnum = ECPPOdaEnum::Back1;
		SwitchStayMove = false;
		WaitTime = 0;
	}
}

void AOdaBase::OdaMove1(int Timer)
{
	//一秒たったら切り替える
	if (Timer % 60 == 0)
	{
		//反転
		SwitchStayMove = true;
	}
	if (SwitchStayMove)
	{
		FlontMove(FastOdaSpeed*2);
	}
	else
	{ 
		//ちょっとだけ下がる
		BackMove(OdaSpeed / 5.f);
	}
	if (GetDistanceTo(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) <= 250) {
		OdaMoveEnum = ECPPOdaEnum::Back1;
		SwitchStayMove = false;
		WaitTime = 0;

	}
}

void AOdaBase::OdaMove2(int Timer)
{
	if (Timer % 40 == 0)
	{
		SwitchStayMove = !SwitchStayMove;
	}
	if (SwitchStayMove)
	{
		FlontMove(FastOdaSpeed*4.f);
	}
	else
	{
		BackMove(OdaSpeed / 10.f);
	}
	if (GetDistanceTo(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) <= 250) {
		OdaMoveEnum = ECPPOdaEnum::Back1;
		SwitchStayMove = false;
		WaitTime = 0;
	}
}

void AOdaBase::OdaBack1(int Timer) {
	if (Timer % 90 == 0)
	{
		OdaMoveEnum = ECPPOdaEnum::Stay1;
		SwitchStayMove = false;
		WaitTime = 0;
	}
	BackMove(FastOdaSpeed * 10.f);

}

float AOdaBase::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;
		if (Health <= 0.f)
		{
			//体力が0以下になったときの処理
			this->Destroy();
		}
	}
	return ActualDamage;
}
void AOdaBase::HPRock(bool _isHPRock)
{

}


void AOdaBase::FlontMove(float Speed)
{
	OdaNobunaga->AddMovementInput(ForwardDirection, Speed);
}

void AOdaBase::BackMove(float Speed)
{
	OdaNobunaga->AddMovementInput(ForwardDirection, -Speed);
}

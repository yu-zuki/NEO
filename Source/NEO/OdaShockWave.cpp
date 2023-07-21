// Fill out your copyright notice in the Description page of Project Settings.


#include "OdaShockWave.h"

// Sets default values
AOdaShockWave::AOdaShockWave() :
	IsRightPlayer(true),
	Scale(FVector(1.f,1.f,1.f)),
	NiagaraComp(NULL)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraCpomponent"));


}

// Called when the game starts or when spawned
void AOdaShockWave::BeginPlay()
{
	Super::BeginPlay();
	
	//�i�C�A�K���G�t�F�N�g�̐���	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Wave, GetActorLocation(), GetActorRotation(), Scale);



	//�v���C���[�̈ʒu���擾
	//�v���C���[���E���ɂ���Ȃ�

	//if (GetActorLocation().Y <= UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
	//{
	//	IsRightPlayer = true;
	//}
	////�v���C���[�������ɂ���Ȃ�
	//else
	//{
	//	SetActorRotation(FRotator(0.f,0.f, 90.f));
	//	IsRightPlayer = false;
	//}
}

// Called every frame
void AOdaShockWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveWave();
}

void AOdaShockWave::MoveWave()
{
	////�v���C���[���E���ɂ���Ȃ�
	//if (IsRightPlayer)
	//{
	//	SetActorRotation(FRotator(0.f, 0.f, 180.f));
	//}
	////�v���C���[�������ɂ���Ȃ�
	//else
	//{

	//}

	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y + 1.f, GetActorLocation().Z));
	UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(GetActorLocation().Y), true, true, FColor::Cyan, 2.f, TEXT("None"));

}


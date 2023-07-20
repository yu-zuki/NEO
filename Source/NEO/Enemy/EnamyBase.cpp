// Fill out your copyright notice in the Description page of Project Settings.


#include "EnamyBase.h"
#include "NEO/GameSystem/TGS_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/GameSystem/Enemy_UMG.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "NEO/GameSystem/EnemyBase_WidgetComponent.h"

// Sets default values
AEnamyBase::AEnamyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
	//UI Create
	EnemyWidget = CreateDefaultSubobject<UEnemyBase_WidgetComponent>(TEXT("EnemyWidget"));
	EnemyWidget->SetupAttachment(RootComponent);
}

void AEnamyBase::DestoryEnemy()
{
	ATGS_GameMode* GameMode =Cast<ATGS_GameMode>( UGameplayStatics::GetGameMode(GetWorld()) );
	if (GameMode)
	{
		GameMode->DestroyEnemy(this,IsAreaEnemy);
	}
}

// Called when the game starts or when spawned
void AEnamyBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();

 
    
}

// Called every frame
void AEnamyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    // �L�����N�^�[�̈ʒu���擾
    FVector CharacterLocation = GetActorLocation();

    // �����̍��W���擾
    FVector MyLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	//Enemy Hp Set
	EnemyWidget->SetHPInfo(Health, MaxHealth);
    // �L�����N�^�[�̈ʒu�Ǝ����̈ʒu���r����Y�����O�ɂ��邩�ǂ����𔻒�
    bIsRotation = CharacterLocation.Y > MyLocation.Y;
    // bIsRotation��true�Ȃ�
    if (bIsRotation)
    {
        FRotator NewRotation = GetActorRotation();
        NewRotation.Yaw = -90.0f;
        SetActorRotation(NewRotation);

    }
    else
    {
        FRotator NewRotation = GetActorRotation();
        NewRotation.Yaw = 90.0f;
        SetActorRotation(NewRotation);
    }


}

// Called to bind functionality to input
void AEnamyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnamyBase::ApplyDamage(float DamageAmount)
{
	Health -= DamageAmount;
    if (Health <= 0)
    {
        PlayAnimMontage(Death, 1, NAME_None);
    }
}




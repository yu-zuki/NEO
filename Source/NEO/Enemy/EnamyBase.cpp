// Fill out your copyright notice in the Description page of Project Settings.


#include "EnamyBase.h"
#include "EngineUtils.h"
#include "NEO/GameSystem/TGS_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/GameSystem/GameSystem_BattleArea.h"
#include "NEO/GameSystem/Enemy_UMG.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h" 
#include "Camera/CameraComponent.h"
#include "NEO/GameSystem/EnemyBase_WidgetComponent.h"
#include "NEO/PlayerSystem/ActionAssistComponent.h"
#include "NEO/PlayerSystem/PlayerBase.h"


// Sets default values
AEnamyBase::AEnamyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//UI Create

	bIsDeath = false;

	// �A�^�b�N�A�V�X�g�R���|�[�l���g�쐬
	ActionAssistComp = CreateDefaultSubobject<UActionAssistComponent>(TEXT("AttackAssist"));
	Damage = 5.0f;

	bHasPattern1Tag = Tags.Contains("pattern1");
	bHasPattern2Tag = Tags.Contains("pattern2");
	bHasPattern3Tag = Tags.Contains("pattern3");
	bHasWeponTag = Tags.Contains("HasWepon");



	MoveSpline = CreateDefaultSubobject<USplineComponent>(TEXT("MoveSpline"));
	MoveSpline->SetupAttachment(RootComponent);
	/*MovementSpline = CreateDefaultSubobject<USplineComponent>(TEXT("MovementSpline1"));
	MovementSpline->SetupAttachment(RootComponent);*/
}

void AEnamyBase::DestoryEnemy()
{
	ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->DestroyEnemy(this, IsAreaEnemy);

	}
	
}

// Called when the game starts or when spawned
void AEnamyBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();

	if (GetMesh())
	{
		AnimInstance = GetMesh()->GetAnimInstance();
	}
	TArray<AActor*> TempBattleAreas;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameSystem_BattleArea::StaticClass(), TempBattleAreas);

	for (AActor* Actor : TempBattleAreas) {
		AGameSystem_BattleArea* BattleArea = Cast<AGameSystem_BattleArea>(Actor);
		if (BattleArea) {

		}
	}
	/*TArray<AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), FoundPlayers);

	if (FoundPlayers.Num() > 0)
	{
		PlayerCharacter = Cast<ACharacter>(FoundPlayers[0]);
	}*/
	//if (ActorHasTag("pattern2")) {
	//	// Yaw�𒆐S��180�x��]������
	//	AddActorLocalRotation(FRotator(0.0f, 180.0f, 0.0f));
	//	// �X�v���C���ɉ����Ĉړ����J�n����
	//	bShouldMoveAlongSpline = true;
	//	TimeSinceStartOfMovement = 0.0f;
	//}
	//TArray<AActor*> FoundBattleAreas;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameSystem_BattleArea::StaticClass(), FoundBattleAreas);

	//for (AActor* Actor : FoundBattleAreas)
	//{
	//	AGameSystem_BattleArea* BattleArea = Cast<AGameSystem_BattleArea>(Actor);
	//	if (BattleArea)
	//	{
	//		// BattleArea�̊e���b�V���R���|�[�l���g�iLeftMesh, RightMesh, NearMesh�j���擾����B
	//		UProceduralMeshComponent* LeftMesh = BattleArea->LeftMesh;
	//		UProceduralMeshComponent* RightMesh = BattleArea->RightMesh;
	//		UProceduralMeshComponent* NearMesh = BattleArea->NearMesh;

	//		// EnamyBase�̃J�v�Z���R���W�����������̃��b�V���R���|�[�l���g�𖳎�����悤�ɐݒ肷��B
	//		if (LeftMesh && RightMesh && NearMesh)
	//		{
	//			CapsuleComponent->SetCollisionResponseToChannel(LeftMesh->GetCollisionObjectType(), ECR_Ignore);
	//			CapsuleComponent->SetCollisionResponseToChannel(RightMesh->GetCollisionObjectType(), ECR_Ignore);
	//			CapsuleComponent->SetCollisionResponseToChannel(NearMesh->GetCollisionObjectType(), ECR_Ignore);
	//		}
	//	}
	//}
}

AActor* AEnamyBase::GetPlayer()
{
	APawn* PlayerChara = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	return PlayerChara;
}

// Called every frame
void AEnamyBase::Tick(float DeltaTime)
{

	if (bIsNowDamage)
	{
		return;
	}

	Super::Tick(DeltaTime);




	//�L�����N�^�[�̈ʒu���擾
	FVector CharacterLocation = GetActorLocation();

	// �����̍��W���擾
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			APawn* Pawn = PlayerController->GetPawn();
			if (Pawn)
			{
				FVector MyLocation = Pawn->GetActorLocation();
				// �L�����N�^�[�̈ʒu�Ǝ����̈ʒu���r����Y�����O�ɂ��邩�ǂ����𔻒�
				bIsRotation = CharacterLocation.Y > MyLocation.Y;
				bIsRotationTag2 = CharacterLocation.X > MyLocation.X;
				//bIsRotation��true�Ȃ�
				if (Health >= 0)
				{

					bool LookRight = false;

					if (ActorHasTag("pattern1"))
					{
						if (bIsRotation)
						{
							/*FRotator NewRotation = GetActorRotation();
							NewRotation.Yaw = -90.0f;
							SetActorRotation(NewRotation);*/


							LookRight = true;
						}
						else
						{
							/*FRotator NewRotation = GetActorRotation();
							NewRotation.Yaw = 90.0f;
							SetActorRotation(NewRotation);*/

							LookRight = false;
						}

						ActionAssistComp->OwnerParallelToCamera(LookRight);
					}
					else if (ActorHasTag("pattern2"))
					{
						if (bIsRotationTag2)
						{
							/*FRotator NewRotation = GetActorRotation();
							NewRotation.Yaw = -180.0f;
							SetActorRotation(NewRotation);*/

							LookRight = true;

						}
						else
						{
							/*FRotator NewRotation = GetActorRotation();
							NewRotation.Yaw = 0.0f;
							SetActorRotation(NewRotation);*/

							LookRight = false;
						}

						ActionAssistComp->OwnerParallelToCamera(LookRight);
					}
					else if (ActorHasTag("pattern3"))
					{
						if (bIsRotation)
						{
							/*FRotator NewRotation = GetActorRotation();
							NewRotation.Yaw = -90.0f;
							SetActorRotation(NewRotation);*/

							LookRight = false;

						}
						else
						{
							/*FRotator NewRotation = GetActorRotation();
							NewRotation.Yaw = 90.0f;
							SetActorRotation(NewRotation);*/

							LookRight = true;
						}

						ActionAssistComp->OwnerParallelToCamera(LookRight);
					}
				}

			}
		}
	}

	if (bHasPattern1Tag && GetWorld()->GetTimeSeconds() - SpawnTime < 4.5f || bHasPattern2Tag || bHasPattern3Tag)
	{
		float TimeSinceSpawn = GetWorld()->GetTimeSeconds() - SpawnTime;
		float SplineDuration = 3.0f;  // �X�v���C������������܂ł̎���
		float SplineProgress = FMath::Clamp(TimeSinceSpawn / SplineDuration, 0.0f, 1.0f);
		FVector NewLocation = MoveSpline->GetLocationAtSplineInputKey(SplineProgress, ESplineCoordinateSpace::World);
		SetActorLocation(NewLocation);
		for (AGameSystem_BattleArea* BattleArea : BattleAreaReferences) {
			if (BattleArea && BattleArea->IsOverlappingActor(this)) {
				BattleArea->IgnoreCollision();
			}
		}
		return; // ����Tick�������X�L�b�v
	}
	else
	{
		for (AGameSystem_BattleArea* BattleArea : BattleAreaReferences) {
			if (BattleArea) {
				BattleArea->ResetCollision();
			}
		}
	}
	if (bShouldMoveAlongSpline) {
		// 3�b�Ԉړ�������ɂ�
		TimeSinceStartOfMovement += DeltaTime;
		float Alpha = FMath::Min(TimeSinceStartOfMovement / MovementDuration, 1.0f);
		FVector NewLocation = MovementSpline->GetLocationAtDistanceAlongSpline(MovementSpline->GetSplineLength() * Alpha, ESplineCoordinateSpace::Local);
		SetActorLocation(NewLocation);

		if (Alpha >= 1.0f) {
			bShouldMoveAlongSpline = false;
		}

		// ����Tick�������~����ɂ�
		return;
	}
	CheckHealth();
}

void AEnamyBase::CheckCollisonOff()
{
	
}
void AEnamyBase::SpawnSword()
{

}


void AEnamyBase::ApplyDamage(float DamageAmount)
{

	Health -= DamageAmount;
	FTimerHandle TimerHandle_ResetDamage;
	bIsNowDamage = true;
	GetWorldTimerManager().SetTimer(TimerHandle_ResetDamage, this, &AEnamyBase::DamageReac, 0.2f, false);
	if (Health <= 0)
	{

		PlayAnimMontage(Death, 0.8, NAME_None);
		GetWorldTimerManager().SetTimer(TimerHandle_DestroyEnemy, this, &AEnamyBase::AfterDeath, 1.6f, true);
		bIsDeath = true;
		FVector ForceDirection = -GetActorForwardVector();
		float ForceStrength = 1000.0f;  // Adjust this value as needed.
		Tags.Remove(FName("Enemy"));

		GetCharacterMovement()->AddForce(ForceDirection * ForceStrength);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);


	}
	else
	{
		PlayAnimMontage(Damage_Reaction, 0.8, NAME_None);

		ActionAssistComp->SpawnEffect(NiagaraEffect, GetActorLocation());

	}


}
void AEnamyBase::MaintainDistanceFromEnemy()
{
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* CurrentActor = *It;
		if (CurrentActor && CurrentActor->ActorHasTag(FName("Enemy")))
		{
			FVector Direction = CurrentActor->GetActorLocation() - GetActorLocation();
			float Distance = Direction.Size();

			if (Distance < DesiredDistanceFromEnemy)
			{
				FVector MoveDirection = -Direction.GetSafeNormal() * (DesiredDistanceFromEnemy - Distance);
				AddMovementInput(MoveDirection);
			}
		}
	}
}
void AEnamyBase::AfterDeath()
{
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	// ������null�`�F�b�N������
	if (GetWorld())
	{
		if (bHasWeponTag)
		{
			AActor* SpawnedEnemySword = GetWorld()->SpawnActor<AActor>(EnemySwordClass, SpawnLocation, SpawnRotation);

		}
	}
	DestoryEnemy();

}

void AEnamyBase::DamageReac()
{
	bIsNowDamage = false;
}

void AEnamyBase::CheckHealth()
{
	if (Health <= 0)
	{
		SpawnDeathTrigger();
	}
}

void AEnamyBase::SpawnDeathTrigger()
{
	if (DeathTriggerClass = nullptr)
	{
		FTransform SpawnTransform = GetActorTransform();
		ADeathTrigger* DeathTrigger = GetWorld()->SpawnActor<ADeathTrigger>(DeathTriggerClass, SpawnTransform);
		if (DeathTriggerClass != nullptr)
		{
			DeathTrigger->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}

bool AEnamyBase::IsAnimationAttacking() const
{
	UAnimInstance* AnimAttack = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimAttack)
	{
		if (AnimAttack->Montage_IsPlaying(Attack) || AnimAttack->Montage_IsPlaying(Attack2) || AnimAttack->Montage_IsPlaying(Attack3))
		{

			return true;
		}
	}
	return false;
}

AActor* AEnamyBase::GetEnemyActor() const
{
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor && Actor->ActorHasTag(FName("Enemy")))
		{
			return Actor; // Enemy�^�O�����ŏ��̃A�N�^�[��Ԃ�
		}
	}
	return nullptr; // Enemy�^�O�����A�N�^�[��������Ȃ��ꍇ
}

void AEnamyBase::OnOverlapBeginWall(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetCapsuleComponent())  // WallCollision �Ɠ���
	{
		// ���� EnemyBase �̃R���W�������X�|���X�� Block �ɐݒ�
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	}
}





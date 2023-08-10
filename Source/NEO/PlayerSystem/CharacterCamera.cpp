// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/SplineComponent.h"
#include "PlayerBase.h"
#include "NEO/GameSystem/GameSystem_BattleArea.h"

#include "NEO/GameSystem/TGS_GameMode.h"

// Sets default values
ACharacterCamera::ACharacterCamera()
	:m_splineTagName("")
	, m_pSplineActor(NULL)
	, m_localLength(0.0f)
	, m_moveDistance(0.0f)
	, m_defaultSpeed(100.0f)
	, m_pPlayer(NULL)
	, m_CanMove(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bInheritPitch = false;
	CameraBoom->bDoCollisionTest = false;

	// Create a follow camera
	Follow_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Follow_Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	Follow_Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

// Called when the game starts or when spawned
void ACharacterCamera::BeginPlay()
{
	Super::BeginPlay();

	ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//AGameSystem_BattleArea BattleArea;

	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);

	
	if (PlayerController)
	{
		PlayerController->SetViewTargetWithBlend(this, 0.f);
	}

	//// �v���C���[�̎擾
	//m_pCharaOwner = Cast<APlayerBase>(PawnOwner);

	/*if (GameMode)
	{
		GameMode->SetViewTargetWithBlend(this, 0.5f);
	}*/

	//PlayerInfo = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//PlayerInfo->SetOwner(this);
	PlayerInfo = GetPlayer();

	// �����ʒu�ݒ�
	/*StartPos = PlayerInfo->GetActorLocation();
	PlayerToViewPointDistance = FVector(200.f,400.f,200.f);
	SetActorLocation(StartPos);*/

	//SetActorRotation(FRotator(-25.0, 0.0, 0.0));

	//------------------------�X�v���C��------------------------
	m_pSplineActor = SplineActorInitialize(this, m_splineTagName);
}

// Called every frame
void ACharacterCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	// �v���C���[���擾���Ă��Ȃ���
	if (m_pPlayer == NULL)
	{
		// �v���C���[�L�����N�^�[�̎擾
		TSubclassOf<APlayerBase> findClass1;
		findClass1 = APlayerBase::StaticClass();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), findClass1, Actors);

		if (Actors.Num())
		{
			m_pPlayer = Cast<APlayerBase>(Actors[0]);

		}

	}

	//ACharacter* tmp_PlayerInfo = NULL;
	//if (tmp_PlayerInfo) return;
	//tmp_PlayerInfo = GetPlayer();

	// �v���C���[�̌��݈ʒu�擾
	if (m_pPlayer ==nullptr)
	{
		return;
	}
	FVector PlayerPos = m_pPlayer->GetActorLocation();
	
	// ���ݍ��W���擾����
	FVector nowpos = GetActorLocation();
	FVector newpos = nowpos;
	
	// �V�������W���Z�o����
	newpos = FMath::VInterpTo(nowpos, PlayerPos, DeltaTime, m_defaultSpeed);

	//SetActorLocation(FVector(StartPos.X - PlayerToViewPointDistance.Z, PlayerPos.Y + PlayerToViewPointDistance.X,StartPos.Z + PlayerToViewPointDistance.Y));

	//------------------------�X�v���C��------------------------
	//SplineActor�����݂���ꍇ
	if (m_pSplineActor != NULL)
	{
		////�ړ������̍X�V
		//if(PlayerPos.Y >= GetActorLocation().Y - 520.0f)			//�v���C���[�̕����E�ɂ���ꍇ
		//	m_moveDistance = m_moveDistance + (m_defaultSpeed * DeltaTime);
		//else if (PlayerPos.Y < GetActorLocation().Y - 480.0f)	//�v���C���[�̕������ɂ���ꍇ
		//	m_moveDistance = m_moveDistance - (m_defaultSpeed * DeltaTime);

		//�ړ������̍X�V
		if (PlayerPos.Y > GetActorLocation().Y - 520.0f && PlayerPos.Y < GetActorLocation().Y - 480.0f)
			return;
		else if (PlayerPos.Y > GetActorLocation().Y - 520.0f)	//�v���C���[�̕����E�ɂ���ꍇ
			m_moveDistance = m_moveDistance + (m_defaultSpeed * DeltaTime);
		else if (PlayerPos.Y < GetActorLocation().Y - 480.0f)	//�v���C���[�̕������ɂ���ꍇ
			m_moveDistance = m_moveDistance - (m_defaultSpeed * DeltaTime);

		
		

		//�X�V��̐V�������W�E��]�������郍�[�J���ϐ�
		FVector newLocation;
		FRotator newRotation;

		//���݂̃X�v���C����̋���������W�A��]���Z�o
		GetCurrentInfo0nSpline(m_pPlayer->DistanceAdvanced * m_defaultSpeed * m_pPlayer->deltaTime, newLocation, newRotation);

		//if (m_CanMove)
		//	GetCurrentInfo0nSpline(m_pPlayer->DistanceAdvanced * m_defaultSpeed * m_pPlayer->deltaTime, newLocation, newRotation);
		//else
		//	GetCurrentInfo0nSpline(m_pPlayer->DistanceAdvanced, newLocation, newRotation);
		//	SetActorLocation(newLocation);

		////���݂̃X�v���C����̋���������W�A��]���Z�o
		//GetCurrentInfo0nSpline(m_moveDistance * m_defaultSpeed, newLocation, newRotation);


		newRotation.Roll = -25.0;
		
		if (!m_CanMove)
			return;

		//�X�V��̍��W�E��]���𔽉f
		SetActorLocationAndRotation(newLocation, newRotation);
	}
}


AActor* ACharacterCamera::SplineActorInitialize(AActor* _pOwnerActor, const FName& _tag)
{
	if (_pOwnerActor == NULL) { return NULL; }

	//�Q�[���S�̂ɑ΂���Actor�̌����R�X�g���������߁ABeginPlay�ň��ۑ����Ă��������ɂ���
	//�����Ώۂ͑S�Ă�Actor
	TSubclassOf<AActor> findClass;
	findClass = AActor::StaticClass();
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(_pOwnerActor->GetWorld(), findClass, actors);


	//�������ʁAActor�������
	if (actors.Num() > 0)
	{
		//����Actor�̒������ԂɌ���
		for (int idx = 0; idx < actors.Num(); idx++)
		{
			AActor* pActor = Cast<AActor>(actors[idx]);

			//�^�O���Ŕ��f����
			//���O�̓G�f�B�^��ŏd�������ꍇ�͓����̖��O���ς�邽�߁A���ĂɂȂ�Ȃ�
			if (pActor->ActorHasTag(_tag))
			{
				//�f�o�b�O�m�F
				FString message = FString("Founded Actor	:") + pActor->GetName();
				UE_LOG(LogTemp, Warning, TEXT("%s"), *message);

				return pActor;

			}
		}
	}
	return NULL;

}

void ACharacterCamera::GetCurrentInfo0nSpline(float _length, FVector& _location, FRotator& _rotation)
{
	//Spline���Ȃ���Ζ���
	if (!m_pSplineActor) { return; }

	//�������čŏ��Ɍ�����SplineComponent�I�u�W�F�N�g����擾
	UActorComponent* pComponent =
		m_pSplineActor->GetComponentByClass(USplineComponent::StaticClass());

	//SplineComponent�Ɍ^�ϊ�
	USplineComponent* pSplineComp = Cast<USplineComponent>(pComponent);

	//SplineComponent��������Ζ���
	if (!pSplineComp) { return; }

	////���[�v���Ȃ�(����������)
	//if (_loop)
	//{
	//	m_localLength = _length;
	//}
	////���[�v����
	//else
	//{
	//	//�X�v���C���S�̂̒����ɍ��킹���䗦�����߁A�ʒu���X�V������
	//	float overallLength = pSplineComp->GetSplineLength();
	//	m_localLength = (float)((int)_length % (int)overallLength);

	//}

	//�X�v���C���S�̂̒����ɍ��킹���䗦�����߁A�ʒu���X�V������
	float overallLength = pSplineComp->GetSplineLength();
	m_localLength = (float)((int)_length % (int)overallLength);


	//���݂̃X�v���C���̈ʒu�ɍ��킹�����W�E��]���̒l���Q�ƂŕԂ�
	_location = pSplineComp->GetLocationAtDistanceAlongSpline(m_localLength,
		ESplineCoordinateSpace::World);
	_rotation = pSplineComp->GetRotationAtDistanceAlongSpline(m_localLength,
		ESplineCoordinateSpace::World);
	m_defaultRRRRRRRRRR = _rotation;
}
ACharacter* ACharacterCamera::GetPlayer()
{
	ACharacter*  tmp_PlayerInfo = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (tmp_PlayerInfo == PlayerInfo) {
		return PlayerInfo;
	}
	else {
		tmp_PlayerInfo->SetOwner(this);
		return tmp_PlayerInfo;
	}
}

//// �L�����N�^�[���Z�b�g����֐�
//void ACharacterCamera::GetCharacterOwner(APlayerBase* Chara)
//{
//	m_pCharaOwner = Chara;
//}

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

		PlayerInfo = GetPlayer();

	//------------------------�X�v���C��------------------------
	m_pSplineActor = SplineActorInitialize(this, m_splineTagName);
}

// Called every frame
void ACharacterCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//Spline���Ȃ���Ζ���
	if (!m_pSplineActor) { return; }

	//�������čŏ��Ɍ�����SplineComponent�I�u�W�F�N�g����擾
	UActorComponent* pComponent =
		m_pSplineActor->GetComponentByClass(USplineComponent::StaticClass());

	//SplineComponent�Ɍ^�ϊ�
	pSplineComp = Cast<USplineComponent>(pComponent);

	//SplineComponent��������Ζ���
	if (!pSplineComp) { return; }

	FVector TargetLocation = GetSplineLocationAtDistance(pSplineComp->GetSplineLength());
	FVector CurrentLocation = GetPlayer()->GetActorLocation();

	FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, m_defaultSpeed);

	SetActorLocation(NewLocation);

	
	
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

FVector ACharacterCamera::GetSplineLocationAtDistance(float Distance)
{
	if (pSplineComp)
	{
		FVector Location = pSplineComp->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		return Location;
	}
	return FVector::ZeroVector;
}
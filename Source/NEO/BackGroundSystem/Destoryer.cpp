// Fill out your copyright notice in the Description page of Project Settings.


#include "Destoryer.h"
#include "ObjectBase.h"
// Sets default values
ADestoryer::ADestoryer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh1;
	Mesh1->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	TransparentSkeletalMesh1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TransparentSkeletalMesh1"));
	TransparentSkeletalMesh1->SetupAttachment(RootComponent);

	TransparentSkeletalMesh2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TransparentSkeletalMesh2"));
	TransparentSkeletalMesh2->SetupAttachment(RootComponent);

	TransparentSkeletalMesh3 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TransparentSkeletalMesh3"));
	TransparentSkeletalMesh3->SetupAttachment(RootComponent);

	TransparentSkeletalMesh4 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TransparentSkeletalMesh4"));
	TransparentSkeletalMesh4->SetupAttachment(RootComponent);

	TransparentSkeletalMesh5 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TransparentSkeletalMesh5"));
	TransparentSkeletalMesh5->SetupAttachment(RootComponent);

	TransparentSkeletalMesh6 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TransparentSkeletalMesh6"));
	TransparentSkeletalMesh6->SetupAttachment(RootComponent);
	ForwardBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	ForwardBoxCollider->SetupAttachment(RootComponent);

	// �ʒu�A�T�C�Y�Ȃǂ������Őݒ�ł���
	ForwardBoxCollider->SetRelativeLocation(FVector(0.0f, 50.0f, 0.0f)); // 100���j�b�g�O����
	ForwardBoxCollider->SetBoxExtent(FVector(5.0f, 20.0f, 20.0f)); // �T�C�Y
	ForwardBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ADestoryer::OnOverlapBegin);
	Damage = 5.f;
}

// Called when the game starts or when spawned
void ADestoryer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestoryer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// X���ɐi��
	FVector RightVector = GetActorRightVector();
	FVector NewLocation = GetActorLocation() + (RightVector * 600 * DeltaTime);  // �X�s�[�h��100�Ƃ���
	SetActorLocation(NewLocation);

	// �o�ߎ��Ԃ��X�V
	MoveTime += DeltaTime;

	// 5�b�o�������~
	if (MoveTime >= MaxMoveTime)
	{
		bShouldMoveForward = false;
	}
}


// Called to bind functionality to input
void ADestoryer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}

void ADestoryer::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// �^�O��"Fence"�Ȃ�_���[�W��^����
		if (OtherActor->ActorHasTag("Fence"))
		{
			
			AObjectBase* FenceActor = Cast<AObjectBase>(OtherActor);
			if (FenceActor)
			{
				FenceActor->ReceiveDamage(Damage); // 20.0f �̓_���[�W��
			}
		}
	}
}
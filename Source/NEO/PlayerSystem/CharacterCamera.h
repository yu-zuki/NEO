// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PawnMovementComponent.h"

#include "CharacterCamera.generated.h"

class APlayerBase;
class AGameSystem_BattleArea;


UCLASS()
class NEO_API ACharacterCamera : public AActor
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Follow_Camera;

	// �v���C���[�̏��
	ACharacter* PlayerInfo;

	// �v���C���[�̏���ۑ����邽�߂̕ϐ�
	TArray<AActor*> Actors;
	APlayerBase* m_pPlayer;
	FVector m_PlayerPos;


	// �����ʒu
	FVector StartPos;

	// �v���C���[���璍���_�̈ʒu�܂ł̋���
	UPROPERTY(EditAnywhere, Category = "PlayerToViewPointDistance")
		FVector PlayerToViewPointDistance;
	
public:	
	// Sets default values for this actor's properties
	ACharacterCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//------------------------�X�v���C��------------------------

public:
	//SplineActor�̏������ݒ�
	AActor* SplineActorInitialize(AActor* _pOwnerActor, const FName& _tag);

	//�X�v���C����̌��݂̈ʒu���擾
	void GetCurrentInfo0nSpline(float _length, FVector& _location, FRotator& _rotation);
	
	UPROPERTY(EditAnywhere, Category = "SubChara")		//�T�u�L�����𓮂��������X�v���C���̃^�O��
		FName m_splineTagName;

	AActor* m_pSplineActor;		//�X�v���C����Actor
	float m_localLength;		//���݃X�v���C����̈ʒu

	UPROPERTY(EditAnywhere, Category = "SubChara")		//���x
		float m_defaultSpeed;		//���x

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "SubChara")		//���x
		float m_moveDistance;		//���x

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "SubChara")		//���x
		FRotator m_defaultRRRRRRRRRR;		//���x

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SubChara")		//���x
		bool m_CanMove;		//���x


	//// �L�����N�^�[���
	//APlayerBase* m_pCharaOwner;
	//// �L�����N�^�[�̃Z�b�g
	//void GetCharacterOwner(APlayerBase* Chara);


protected:
	ACharacter* GetPlayer();

	class USplineComponent* pSplineComp;

	FVector GetSplineLocationAtDistance(float Distance);
};

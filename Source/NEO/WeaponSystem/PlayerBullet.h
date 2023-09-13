// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerBullet.generated.h"

UCLASS()
class NEO_API APlayerBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �ڐG�J�n���ɍs������
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	// �e�̈ړ�����
	void Move();

	// �폜�܂ł̎��ԂɒB���Ă�����
	void SetDestroyFlag();

	// �e��Destroy
	void DestroyBullet(bool _destroyFlag = false);

private:

	// �e�̃R���W����
	UPROPERTY(EditAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* BulletCollision;

	// ����̃��b�V��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BulletMesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BulletStaticMesh;

	// �e�̈ړ����x
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BulletParams", meta = (AllowPrivateAccess = "true"))
		float MoveSpeed;

	// ���˂���폜�܂ł̎���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BulletParams", meta = (AllowPrivateAccess = "true"))
		float TimeToDelete;

	// �^�C�}�[�n���h��
	FTimerHandle DeleteTimerHandle;
};

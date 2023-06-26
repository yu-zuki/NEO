// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameSystem_BattleArea.generated.h"

struct SFrustumVertices
{
	FVector TopLeftNear;
	FVector TopRightNear;
	FVector BottomLeftNear;
	FVector BottomRightNear;
	FVector TopLeftFar;
	FVector TopRightFar;
	FVector BottomLeftFar;
	FVector BottomRightFar;
};

UCLASS()
class NEO_API AGameSystem_BattleArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameSystem_BattleArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleArea")
		class USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleArea")
		class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleArea")
		class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleArea")
		float MaxCameraDistance = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleArea")
		float NearPlane = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleArea")
		float FarPlane = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleAreaDebug")
		float lifeTime = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleAreaDebug")
	    float thickness = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleArea")
		class UProceduralMeshComponent* LeftMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleArea")
		class UProceduralMeshComponent* RightMesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CreateBattleArea(); //�o�g���G���A�𐶐�����

	inline FMinimalViewInfo GetCameraViewInfo(); //�J�����̎�����擾����

	SFrustumVertices GetFrustumVertices(FMinimalViewInfo CameraView); //�o�g���G���A�̃��b�V���𐶐�����

	void CreateAreaMesh(SFrustumVertices FrustumVertices); //���b�V���𐶐�����

	void AreaDebugDraw(SFrustumVertices FrustumVertices); //�f�o�b�O�p�̕`��

	//�I�[�o�[���b�v�C�x���g
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

};

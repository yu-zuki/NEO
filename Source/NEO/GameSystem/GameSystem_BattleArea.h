// 更新日：2023/6/24	更新者：董		更新内容：バトルエリア機能が必要な変数を作成
// 更新日：2023/6/26	更新者：董		更新内容：バトルエリア機能がが完成
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

	//カメラの近平面と遠平面の距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleArea")
		float NearPlane = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleArea")
		float FarPlane = 500.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "BattleArea")
		FName EnterActorTag = "Player";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleAreaDebug")
		float lifeTime = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleAreaDebug")
	    float thickness = 1.0f;


	//バトルエリアのメッシュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleArea")
		class UProceduralMeshComponent* LeftMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleArea")
		class UProceduralMeshComponent* RightMesh;

	//近い側のメッシュ
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleArea")
		class UProceduralMeshComponent* NearMesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CreateBattleArea(); //バトルエリアを生成する

	inline FMinimalViewInfo GetCameraViewInfo(); //カメラの視野を取得する

	SFrustumVertices GetFrustumVertices(FMinimalViewInfo CameraView); //バトルエリアのメッシュを生成する

	void CreateAreaMesh(SFrustumVertices FrustumVertices); //メッシュを生成する

	void AreaDebugDraw(SFrustumVertices FrustumVertices); //デバッグ用の描画

	//オーバーラップイベント
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

};

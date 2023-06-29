// 更新日：2023/6/24	更新者：董		更新内容：バトルエリア機能が必要な変数を作成
// 更新日：2023/6/26	更新者：董		更新内容：バトルエリア機能がが完成

#include "GameSystem_BattleArea.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Camera/CameraComponent.h"
#include "../PlayerCharacter.h"

#include "ProceduralMeshComponent.h"
#include "TGS_GameMode.h"

// Sets default values
AGameSystem_BattleArea::AGameSystem_BattleArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);

	//StaticMesh インスタンス
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	//Meshの生成
	LeftMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("LeftProceduralMesh"));
	LeftMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	RightMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("RightProceduralMesh"));
	RightMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AGameSystem_BattleArea::BeginPlay()
{
	Super::BeginPlay();

	auto Transform = GetActorTransform();

	// RootのTransformを初期化
	GetRootComponent()->SetRelativeTransform(FTransform::Identity);

	CreateBattleArea();

	GetRootComponent()->SetRelativeTransform(Transform);

	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AGameSystem_BattleArea::BeginOverlap);
}

// Called every frame
void AGameSystem_BattleArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AGameSystem_BattleArea::CreateBattleArea()
{
	FMinimalViewInfo CameraViewInfo = GetCameraViewInfo();	//カメラビューの情報
	SFrustumVertices FrustumVertices = GetFrustumVertices(CameraViewInfo);	//カメラビューの頂点情報
	CreateAreaMesh(FrustumVertices);	//メッシュの生成
	
	//デバッグ用
	AreaDebugDraw(FrustumVertices);
}

// 
FMinimalViewInfo AGameSystem_BattleArea::GetCameraViewInfo()
{
	FMinimalViewInfo CameraView;	//カメラビューの情報

	FMatrix ViewMatrix;				//ビュー行列
	FMatrix ProjectionMatrix;		//投影行列
	FMatrix ViewProjectionMatrix;	//ビュー投影行列

	CameraComponent->GetCameraView(0, CameraView);	//カメラビューの情報を取得

	//ビュー投影行列を取得
	UGameplayStatics::GetViewProjectionMatrix(CameraView, ViewMatrix, ProjectionMatrix, ViewProjectionMatrix);

	return CameraView;
}

SFrustumVertices AGameSystem_BattleArea::GetFrustumVertices(FMinimalViewInfo CameraView)
{
	//ビューフラスタムの近い平面の半分の幅を計算する
	float HalfWidth = FMath::Tan(FMath::DegreesToRadians(CameraView.FOV) / 2.0f) * NearPlane;
	float HalfHeight = HalfWidth / CameraView.AspectRatio;

	//カメラの位置と向きを取得する
	FVector CameraLocation = CameraView.Location;
	FVector CameraForward = CameraComponent->GetForwardVector();
	FVector CameraRight = CameraComponent->GetRightVector();
	FVector CameraUp = CameraComponent->GetUpVector();

	//視錐台の近い平面の頂点を計算する
	FVector BottomLeftNear = CameraLocation + CameraForward * NearPlane - CameraRight * HalfWidth - CameraUp * HalfHeight;
	FVector TopRightNear = CameraLocation + CameraForward * NearPlane + CameraRight * HalfWidth + CameraUp * HalfHeight;
	FVector BottomRightNear = CameraLocation + CameraForward * NearPlane + CameraRight * HalfWidth - CameraUp * HalfHeight;
	FVector TopLeftNear = CameraLocation + CameraForward * NearPlane - CameraRight * HalfWidth + CameraUp * HalfHeight;

	//ビューフラスタムの遠い平面の半分の幅を計算する
	HalfWidth = FMath::Tan(FMath::DegreesToRadians(CameraView.FOV) / 2.0f) * FarPlane;
	HalfHeight = HalfWidth / CameraView.AspectRatio;

	//視錐台の遠い平面の頂点を計算する
	FVector BottomLeftFar = CameraLocation + CameraForward * FarPlane - CameraRight * HalfWidth - CameraUp * HalfHeight;
	FVector TopRightFar = CameraLocation + CameraForward * FarPlane + CameraRight * HalfWidth + CameraUp * HalfHeight;
	FVector BottomRightFar = CameraLocation + CameraForward * FarPlane + CameraRight * HalfWidth - CameraUp * HalfHeight;
	FVector TopLeftFar = CameraLocation + CameraForward * FarPlane - CameraRight * HalfWidth + CameraUp * HalfHeight;
	
	//最適化してない
	SFrustumVertices FrustumVertices;
	FrustumVertices.BottomLeftNear = BottomLeftNear;
	FrustumVertices.TopRightNear = TopRightNear;
	FrustumVertices.BottomRightNear = BottomRightNear;
	FrustumVertices.TopLeftNear = TopLeftNear;
	FrustumVertices.BottomLeftFar = BottomLeftFar;
	FrustumVertices.TopRightFar = TopRightFar;
	FrustumVertices.BottomRightFar = BottomRightFar;
	FrustumVertices.TopLeftFar = TopLeftFar;

	return FrustumVertices;
}

/**
 * @brief ビューフラスタムを使って2つのメッシュを作成し、それぞれ左辺と右辺に配置する
 *
 * @param FrustumVertices 視錐台の頂点。メッシュ作成に使用される
 */
void AGameSystem_BattleArea::CreateAreaMesh(SFrustumVertices FrustumVertices)
{
	//　左側の平面の頂点を追加
	TArray<FVector> Vertices;
	Vertices.Add(FrustumVertices.BottomLeftNear);
	Vertices.Add(FrustumVertices.TopLeftNear);
	Vertices.Add(FrustumVertices.TopLeftFar);
	Vertices.Add(FrustumVertices.BottomLeftFar);

	//　Verticesの配列を作成
	TArray<int32> Triangles;
	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(2);
	Triangles.Add(2);
	Triangles.Add(3);
	Triangles.Add(0);

	// メッシュの作成
	LeftMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);

	//　レンダーしない
	LeftMesh->SetVisibility(false);

	//　衝突を無効に
	LeftMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//　右側の平面の頂点を追加
	Vertices.Reset();
	Vertices.Add(FrustumVertices.BottomRightNear);
	Vertices.Add(FrustumVertices.TopRightNear);
	Vertices.Add(FrustumVertices.TopRightFar);
	Vertices.Add(FrustumVertices.BottomRightFar);

	//
	RightMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	
	// 
	RightMesh->SetVisibility(false);

	// 
	RightMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGameSystem_BattleArea::AreaDebugDraw(SFrustumVertices FrustumVertices)
{
	//視錐台の頂点を描画する
	DrawDebugLine(GetWorld(), FrustumVertices.BottomLeftNear, FrustumVertices.TopLeftNear, FColor::Red, false, lifeTime, 0, thickness);
	DrawDebugLine(GetWorld(), FrustumVertices.TopLeftNear, FrustumVertices.TopRightNear, FColor::Red, false, lifeTime, 0, thickness);
	DrawDebugLine(GetWorld(), FrustumVertices.TopRightNear, FrustumVertices.BottomRightNear, FColor::Red, false, lifeTime, 0, thickness);
	DrawDebugLine(GetWorld(), FrustumVertices.BottomRightNear, FrustumVertices.BottomLeftNear, FColor::Red, false, lifeTime, 0, thickness);

	DrawDebugLine(GetWorld(), FrustumVertices.BottomLeftFar, FrustumVertices.TopLeftFar, FColor::Red, false, lifeTime, 0, thickness);
	DrawDebugLine(GetWorld(), FrustumVertices.TopLeftFar, FrustumVertices.TopRightFar, FColor::Red, false, lifeTime, 0, thickness);
	DrawDebugLine(GetWorld(), FrustumVertices.TopRightFar, FrustumVertices.BottomRightFar, FColor::Red, false, lifeTime, 0, thickness);
	DrawDebugLine(GetWorld(), FrustumVertices.BottomRightFar, FrustumVertices.BottomLeftFar, FColor::Red, false, lifeTime, 0, thickness);

	DrawDebugLine(GetWorld(), FrustumVertices.BottomLeftNear, FrustumVertices.BottomLeftFar, FColor::Red, false, lifeTime, 0, thickness);
	DrawDebugLine(GetWorld(), FrustumVertices.TopLeftNear, FrustumVertices.TopLeftFar, FColor::Red, false, lifeTime, 0, thickness);
	DrawDebugLine(GetWorld(), FrustumVertices.TopRightNear, FrustumVertices.TopRightFar, FColor::Red, false, lifeTime, 0, thickness);
	DrawDebugLine(GetWorld(), FrustumVertices.BottomRightNear, FrustumVertices.BottomRightFar, FColor::Red, false, lifeTime, 0, thickness);
}

void AGameSystem_BattleArea::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character)	{
		if (Character->ActorHasTag(EnterActorTag))		{
			StaticMeshComponent->DestroyComponent();	// コンポーネントを破壊

			ATGS_GameMode* GameMode = GetWorld()->GetAuthGameMode<ATGS_GameMode>();
			if (ensure(GameMode))			{
				GameMode->SetIsOnBattleArea(true, this, LeftMesh, RightMesh);
			}
		}
	}
}


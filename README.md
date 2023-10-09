# NEO

- **開発形態**: チーム制作
- **ジャンル**: 3Dベルトスクロールアクション
- **開発期間**: 2023年5月1日 ～ 2023年9月24日
- **担当範囲**: `GameSystem`フォルダ内の全て。具体的には以下の項目を担当しました。
  - **GameMode処理**: ゲームの基本ルールや状態の管理。
  - **画面遷移処理**: ゲームの画面間の遷移を制御。
  - **バトルエリア処理**: カメラを固定し、プレイヤーが画面外に出ないようにする処理。
  - **UI全般処理**: ゲーム内のユーザーインターフェースの制御。
  - **SplineCamera処理**: Lineに沿ってカメラを移動させる処理。

[→プレイ動画](#プレイ動画)

[→実行ファイルダウンロード](#実行ファイルのダウンロード)
## 1. プロジェクトの概要

このプロジェクトは学校の研究ゼミでチームを組んで制作されました。目的は東京ゲームショウ(TGS)に参加することであり、企画段階では多くの先生のフィードバックを受けてアイデアを練り直しました。最終的には、3Dベルトスクロールアクションというジャンルを選択しました。プロジェクトのコンセプトは、古いジャンルを現代的にアップデートすることでありました。

## 2. 技術的詳細
- **プログラミング言語**: C++
- **フレームワーク/エンジン**: Unreal Engine 5.1.1
- **使用ツール**: Visual Studio, git, github

## 3. 機能説明
 
### GameMode処理
敵の生成や破壊などの基本的なゲームロジックを制御します。

[→GameMode処理のコードを参照](#GameMode処理のコード部分)

![動画](Document/Battle.gif)


### 画面遷移処理  
タイトル画面からオープニングへ、そしてオープニングからインゲームへの遷移を管理します。各種遷移処理はGameStateで制御されています。

[→画面遷移処理のコードを参照](#画面遷移処理の部分)

![動画](Document/Destiny.gif)


### バトルエリア処理
カメラを固定し、プレイヤーが画面外に出ないように制限します。さらに敵を生成し、カメラの切り替え処理も行います。

[→バトルエリア処理のコードを参照](#バトルエリア処理のコード部分)

![動画](Document/Enemy.gif)


### UI全般処理
敵のUI表示やタイトル画面の動画再生など、ゲーム内のユーザーインターフェースを管理します。

[→UI全般処理のコードを参照](#UI全般処理のコード部分)

![動画](Document/UI.gif)


### SplineCamera処理
スプライン曲線に沿ってカメラを滑らかに移動させる処理を実装しています。

[→SplineCamera処理のコードを参照](#SplineCamera処理のコード部分)

![動画](Document/Quest.gif)


### 簡単な仕様図
![簡単な説明図](Document/ARPG.png)


## 4. ソースコードの説明とサンプル

### GameMode処理のコード部分
プレイヤーと敵の生成や破壊などの基本的なゲームロジックを制御します。

```cpp
//ファイル：TGS_GameMode.cpp
void ATGS_GameMode::SpawnPlayer(AActor* _player, FTransform _tranceform)
{
	GetWorld()->SpawnActor<AActor>(PlayerBaseClass, _tranceform);
}

void ATGS_GameMode::DestroyPlayer(AActor* _player)
{
	DeathTrans = _player->GetActorTransform();
	_player->Destroy();
}

void ATGS_GameMode::RespawnPlayer()
{
	//プレイヤーを生成する
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		PlayerPtr = Cast<APlayerBase>(GetWorld()->SpawnActor<APawn>(DefaultPawnClass, DeathTrans));
		PlayerController->Possess(PlayerPtr);

		SetViewTargetWithBlend(GetCameraActor());

	}
}

void ATGS_GameMode::SpawnEnemyInBattleArea()
{
	//Check SpawnPoints
	if (GetGameState()->BattleAreaSpawnPoints.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("SpawnPoints is not found"));
		return;
	}

	//スポーン
	UWorld* World = GetWorld();
	if (!World) return;


	for (ASpawnPoint* spawnPoint : GetGameState()->BattleAreaSpawnPoints) {
		if (!spawnPoint) continue; //Check SpawnPoint

		//敵を生成する
		SpawnEnemy(spawnPoint);
	}
}

void ATGS_GameMode::SpawnEnemy(AActor* _enemy, FTransform _tranceform)
{
	//敵を生成する
	AActor* enemy = GetWorld()->SpawnActor<AActor>(EnemyBaseClass, _tranceform);

	//敵をゲームステートに登録する
	GetGameState()->AddEnemy(enemy);
}

AActor* ATGS_GameMode::SpawnEnemy(ASpawnPoint* spawnPoint)
{
	//NULL Check
	if (!spawnPoint) {
		UE_LOG(LogTemp, Error, TEXT("SpawnPoint is not found"));
		return nullptr;
	}

	//Transformを取得する
	FTransform spawnTransform = spawnPoint->GetTransform();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* spawn_Actor = GetWorld()->SpawnActor<AActor>(spawnPoint->GetSpawnActorClass(), spawnPoint->GetActorLocation(), spawnPoint->GetActorRotation(), SpawnParams);


	AOdaBase* Boss = Cast<AOdaBase>(spawn_Actor);
	AEnamyBase* Enemy = Cast<AEnamyBase> (spawn_Actor);

	if (Enemy)	{
		Enemy->SetActorTransform(spawnTransform);
		Enemy->IsAreaEnemy = true;		//Flag Set 

		//敵をゲームステートに登録する
		GetGameState()->AddEnemy(Enemy);

		GetGameState()->BattleAreaEnemyCount += 1;
		return Enemy;
	}
	else if(Boss){
		Boss->SetActorTransform(spawnTransform);
		Boss->IsAreaEnemy = true;		//Flag Set 

		//敵をゲームステートに登録する
		GetGameState()->AddEnemy(Boss);

		GetGameState()->BattleAreaEnemyCount += 1;
		return Boss;
	}

	return nullptr;

}

void ATGS_GameMode::DestroyEnemy(AActor* _enemy,bool BattleAreaEnemy)
{
	//敵をゲームステートから削除する
	GetGameState()->RemoveEnemy(_enemy);

	if(BattleAreaEnemy) {
		GetGameState()->BattleAreaEnemyCount -= 1;
	}
}

void ATGS_GameMode::ClearEnemy()
{
	//敵をゲームステートから削除する
	GetGameState()->ClearEnemies();
}

```

### 画面遷移処理の部分
レベルの名前を設定して、画面遷移を行います。

```cpp
//ファイル：TGS_GameStateBase.cpp

void ATGS_GameStateBase::OnGameTitle()
{
	ESubAction currentSubAction = UseSubAction();					


	if (currentSubAction != ESubAction::ESubAction_None) {

		SetCurrentState(EGameState::EGame_Opening);
		GetGameInstance()->SaveGameStateData(ECurrentState);		//インスタンスにゲームの状態を保存
		GetGameInstance()->SavePlayerType(ECurrentPlayerType);		//インスタンスにPlayerTypeを保存

		GetGameInstance()->SaveRemainingLife(Life);					//PlayerLifeReset

		ChangeNextLevel(ENextLevel::ENextLevel_Opening);
	}

}

void ATGS_GameStateBase::ChangeNextLevel(ENextLevel NextLevel)
{
	switch (NextLevel)
	{
	case ENextLevel::ENextLevel_None:
		break;
	case ENextLevel::ENextLevel_Title:
		ChangeLevel(GameTitleLevelName);
		break;
	case ENextLevel::ENextLevel_Opening:
		ChangeLevel(GameOpeningLevelName);
		break;
	case ENextLevel::ENextLevel_Playing:
		ChangeLevel(GamePlayLevelName);
		break;
	case ENextLevel::ENextLevel_Over:
		ChangeLevel(GameOverLevelName);
		break;
	case ENextLevel::ENextLevel_Clear:
		ChangeLevel(GameClearLevelName);
		break;
	}
}

void ATGS_GameStateBase::ChangeLevel(FName NextLevelName)
{
	//次のレベルに移動する
	if (NextLevelName != NAME_None) {
		UGameplayStatics::OpenLevel(GetWorld(), NextLevelName);
	}
}
```


### バトルエリア処理のコード部分

プレイヤーがバトルエリアに入ると、`UProceduralMeshComponent`を使用してリアルタイムでメッシュが生成される処理が行われます。

具体的には、カメラの視野角を取得し、各頂点データの座標を計算します。頂点配列を1つ作成し、それを使用して2つの三角形のメッシュを生成します。

この処理は、バトルエリアの左側、右側、および近い平面でそれぞれ行われます。メッシュの生成が完了した後、敵がスポーンします。


```cpp
//ファイル：GameSystem_BattleArea.cpp

// Sets default values
AGameSystem_BattleArea::AGameSystem_BattleArea()
	//:NowBattleArea(false)
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
	StaticMeshComponent->SetVisibility(false);

	//Meshの生成
	LeftMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("LeftProceduralMesh"));
	LeftMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	RightMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("RightProceduralMesh"));
	RightMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	
	NearMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("NearProceduralMesh"));
	NearMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}


// Called when the game starts or when spawned
void AGameSystem_BattleArea::BeginPlay()
{
	Super::BeginPlay();


	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AGameSystem_BattleArea::BeginOverlap);

	GetSpawnPoints();
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


	// 
	RightMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//近い平面の頂点を追加
	Vertices.Reset();
	Vertices.Add(FrustumVertices.BottomLeftNear);
	Vertices.Add(FrustumVertices.TopLeftNear);
	Vertices.Add(FrustumVertices.TopRightNear);
	Vertices.Add(FrustumVertices.BottomRightNear);

	//
	NearMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	NearMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LeftMesh->SetVisibility(false);
	RightMesh->SetVisibility(false);
	NearMesh->SetVisibility(false);

}

void AGameSystem_BattleArea::AreaDebugDraw(SFrustumVertices FrustumVertices)
{
	//視錐台の頂点を描画する
	
}

void AGameSystem_BattleArea::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character)	{
		if (Character->ActorHasTag(EnterActorTag))		{
			StaticMeshComponent->DestroyComponent();	// コンポーネントを破壊

			BPFunction();

			EnterBattleArea();

			//NowBattleArea = true;
		}
	}
}

int32 AGameSystem_BattleArea::CheckEnemyCount()
{
	return 0;
}

void AGameSystem_BattleArea::EnterBattleArea()
{
	//カメラをコンポーネントを別のカメラに設定
	ATGS_GameMode* GameMode = GetWorld()->GetAuthGameMode<ATGS_GameMode>();

	//Transformを設定
	UCameraComponent* NowCamera = GameMode->GetCameraActor()->FindComponentByClass<UCameraComponent>();
	CameraComponent->SetWorldTransform(NowCamera->GetComponentTransform());
	CameraComponent->FieldOfView = NowCamera->FieldOfView;
	CameraComponent->AspectRatio = NowCamera->AspectRatio;


	//Meshの生成
	auto Transform = GetActorTransform();

	// RootのTransformを初期化
	GetRootComponent()->SetRelativeTransform(FTransform::Identity);

	CreateBattleArea();

	GetRootComponent()->SetRelativeTransform(Transform);

	//GameModeに入ります

	if (GameMode) {
		bIsInBattleArea = true;
		if (SpawnPoints.Num() == 0) {
			UE_LOG(LogTemp, Warning, TEXT("SpawnPoints is empty"));
			return;
		}

		GameMode->SetIsOnBattleArea(bIsInBattleArea, SpawnPoints, this, LeftMesh, RightMesh, NearMesh);

	}
}

```

### UI全般処理のコード部分

```cpp
//ファイル：DestinyChangerCharacter.h

```

### SplineCamera処理
スプライン曲線に沿ってカメラを滑らかに移動させる処理を実装しています。

```cpp
//ファイル：QuestSystem.h

```

## 5. 成果と学び


## 6. ビジュアルとメディア
![スクリーンショット](Document/Title.png)  

### プレイ動画
[プロジェクトデモビデオ]()

## 7. プロジェクトのリンク
- [GitHubリポジトリ](https://github.com/yu-zuki/NEO)  
### 実行ファイルのダウンロード
- [実行可能なデモ Github](https://github.com/yu-zuki/DestinyChanger/releases/tag/Demov1.0)
- [実行可能なデモ GoogleDrive](https://drive.google.com/drive/folders/1w2h7GhYy8HUFfTIW6YDCepHEAFAgkzOj?usp=drive_link)

## 8. 連絡先情報
- 名前: トウ　ゼイキ
- メール: touzeiki@gmail.com


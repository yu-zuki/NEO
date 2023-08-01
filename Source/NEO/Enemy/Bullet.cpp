#include "Bullet.h"
#include "GameFramework/Character.h"
#include "TrajectoryBullet.h"
#include "NEO/PlayerSystem/PlayerBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABullet::ABullet()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;
    // Create a sphere component and attach it to the root component
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    RootComponent = SphereComponent;

    // Set up collision event
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap);

    Lifespan = 0.5f;     // Set your desired lifespan here
    Direction = FVector(1, 0, 0); // Bullet moves in the X direction by default
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(Lifespan);
   
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}
void ABullet::EnableCollision()
{
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        //�G�ꂽ�̂��L�����N�^�[��
        APlayerBase* PlayerCharacter = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(this, 0));
        if (OtherActor == PlayerCharacter)
        {
            // Apply damage to the player
            PlayerCharacter->TakedDamage(DamageAmount);

            OverlappedComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            FTimerHandle UnusedHandle;
            GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABullet::EnableCollision, 3.0f, false);
        }
    }
}

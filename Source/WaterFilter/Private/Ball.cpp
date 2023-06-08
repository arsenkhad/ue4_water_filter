#include "..\Public\Ball.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultBallComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Filter Ball"));
	SetRootComponent(DefaultBallComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	UStaticMesh* Mesh = MeshAsset.Object;
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(
		TEXT("Material'/Game/MBasicMaterial.MBasicMaterial'"));
	Material = MaterialAsset.Object;
	DefaultBallComponent->SetStaticMesh(Mesh);
	DefaultBallComponent->SetWorldScale3D(FVector(0.01f));
	DefaultBallComponent->SetMaterial(0, Material);
	DefaultBallComponent->SetSimulatePhysics(true);
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


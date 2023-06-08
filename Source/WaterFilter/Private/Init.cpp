#include "Init.h"

AInit::AInit()
{
	PrimaryActorTick.bCanEverTick = true;
	SpawnParameters.Owner = this;
}

void AInit::BeginPlay()
{
	Super::BeginPlay();
	FilterReady = false;
}

void AInit::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (Filter == nullptr)
	{
		Filter = GetWorld()->SpawnActor<AFilterCylinder>(AFilterCylinder::StaticClass(), FTransform(), SpawnParameters);
		Filter->CreateCylinder(0, InnerCylinderD/2, CylinderH, CylindersDelta);
		Filter->CreateCylinder(1, OuterCylinderD/2, CylinderH, NULL, true);
	}

	if (!FilterBalls.Num())
		GenerateCloud(FilterBalls, CylinderH * 8, OuterCylinderD * 2, SphereDiameter, SphereDiameterTolerance);
}

void AInit::Destroyed()
{
	Super::Destroyed();
	if (Filter != nullptr)
		Filter->Destroy();
	for(const auto Ball : FilterBalls)
		if (Ball != nullptr)
			Ball->Destroy();
}

void AInit::GenerateCloud(TArray<ABall*> &Cloud, const float CloudHeight, const float CloudSize, const float BallDiameter, const float OffsetD = 0) const
{
	const FVector BallsOffset = FVector(0, 0, CylinderH * 1.5f);
	const float CloudR = CloudSize / 2;
	
	for (int i=-CloudR; i<CloudR; i+=BallDiameter*2)
		for (int j=-CloudR; j<CloudR; j+=BallDiameter*2)
			for (int k=0; k<CloudHeight; k+=BallDiameter*2)
			{
				FTransform TransformSetting = FTransform(FRotator(0.f), BallsOffset + FVector(i, j, k), FVector(BallDiameter + DiameterRandomize(OffsetD)));
				Cloud.Emplace(GetWorld()->SpawnActor<ABall>(ABall::StaticClass(), TransformSetting, SpawnParameters));
			}
}

void AInit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FilterReady && GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::SpaceBar))
	{
		for(const auto Ball : FilterBalls)
		{
			const FVector Coords = Ball->GetActorLocation();
			if (Coords.Size2D() > OuterCylinderD/2 || Coords.Z < 0 || Coords.Z > CylinderH)
				Ball->Destroy();
			else
				Ball->DisableComponentsSimulatePhysics();
		}
		Filter->Destroy();
		FilterReady = true;
		ABall* WaterBall = NewObject<ABall>(this, ABall::StaticClass());
		return;
	}

	if (FilterReady && GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::SpaceBar))
		GenerateCloud(WaterBalls, ParticlesH, OuterCylinderD, WaterParticleDiameter);
}

// ABall* Ball = NewObject<ABall>(this, ABall::StaticClass());
// Ball->SetActorRelativeTransform(TransformSetting);
// FilterBalls.Emplace(Ball);
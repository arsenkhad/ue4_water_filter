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
	ParseXML("D:/'Unreal Engine'/Unreal_Projects/WaterFilter/Config/config.xml");
	
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
	if (!FilterReady && GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(ActivationKey))
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
	}
}

void AInit::ParseXML(const FString& Path)
{
	FXmlFile* File = new FXmlFile(Path, EConstructMethod::ConstructFromFile);
	const FXmlNode* RootNode = File->GetRootNode();
	if (RootNode == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("XML file open failed. Using defaults.") );
	}
	else
	{
		TMap<FString, float*> Attributes;
		Attributes.Add("SphereDiameter", &SphereDiameter);
		Attributes.Add("SphereDiameterTolerance", &SphereDiameterTolerance);
		Attributes.Add("InnerCylinderD", &InnerCylinderD);
		Attributes.Add("OuterCylinderD", &OuterCylinderD);
		Attributes.Add("CylinderH", &CylinderH);
		Attributes.Add("CylindersDelta", &CylindersDelta);
		for(const auto &Attribute : RootNode->GetAttributes())
		{
			FString ValueString = Attribute.GetValue();
			float Value = FCString::Atof(*ValueString);
			Attributes[Attribute.GetTag()] = &Value;
		}
	}
}

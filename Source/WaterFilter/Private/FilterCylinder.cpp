#include "FilterCylinder.h"

AFilterCylinder::AFilterCylinder()
{
	//PrimaryActorTick.bCanEverTick = true;
	CylinderMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = CylinderMesh;
	CylinderMesh->bUseAsyncCooking = true;
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(
		TEXT("Material'/Game/MBasicMaterial.MBasicMaterial'"));
	Material = MaterialAsset.Object;
	CylinderMesh->SetMaterial(0, Material);
	CylinderMesh->SetMaterial(1, Material);
}

void AFilterCylinder::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFilterCylinder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int SetPoint(int &Point, const bool Decrease=false, const bool Prefix = false)
{
	return Prefix ? (Decrease ? --Point : ++Point) : Decrease ? Point-- : Point++;
}

void AFilterCylinder::CreateCylinder(const int ID, const float Radius, const float Height, float Offset, const bool VisibleInside, const int Resolution) const
{
	if (Radius <= 0 || Resolution < 2)
		return;
	
	if (!Offset)
		Offset = 0.f;
	
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;
	
	const float Step = 2 * PI / Resolution;
	float Pos = 0;
	int Point = 2;

	Vertices.Emplace(PolarPoint(0.f, 0.f, Offset));
	Vertices.Emplace(PolarPoint(0.f, 0.f, Height));
	
	Normals.Emplace(PolarPoint(0.f, 0.f, 1.f));
	Normals.Emplace(PolarPoint(0.f, 0.f, -1.f));

	FProcMeshTangent Tangent = FProcMeshTangent(PolarPoint(1.f, 0.f, 0.f), false);
	
	for (int j=0; j<2; j++)
	{
		Tangents.Emplace(Tangent);
		UV0.Emplace(FVector2D(0, 0));
		VertexColors.Emplace(FLinearColor(0.75, 0.75, 0.75, 1.0));
	}
	
	Vertices.Emplace(PolarPoint(Radius, 0.f, Offset));
	Vertices.Emplace(PolarPoint(Radius, 0.f, Height));
	FVector Normal = PolarPoint(Radius, PI, Offset);
    Tangent = FProcMeshTangent(PolarPoint(Radius, PI/2, Offset), false);
 
    for (int j=0; j<2; j++)
    {
        Normals.Emplace(Normal);
        Tangents.Emplace(Tangent);
        UV0.Emplace(FVector2D(0, 0));
        VertexColors.Emplace(FLinearColor(0.75, 0.75, 0.75, 1.0));
    }
	
	
	for (int i=1; i<=Resolution; i++) {
		Point = i*2;
		Pos += Step;
		int Mod = 0;
		Vertices.Emplace(PolarPoint(Radius, Pos, Offset));
		Vertices.Emplace(PolarPoint(Radius, Pos, Height));
		
		for (int j=0; j<6; j++)
			Triangles.Emplace(SetPoint(Point, static_cast<bool>(j / 3), VisibleInside));

		if (VisibleInside)
			Point += 3;
		for (int j=0; j < 2 - VisibleInside; j++)
		{
			Triangles.Emplace(Mod);
			Triangles.Emplace(SetPoint(Point, static_cast<bool>(j - VisibleInside), VisibleInside));
			SetPoint(Point, static_cast<bool>(j - VisibleInside), VisibleInside);
			Triangles.Emplace(SetPoint(Point, static_cast<bool>(j - VisibleInside), VisibleInside));
			Mod = !Mod;
		}
		
		Normal = PolarPoint(Radius, Pos+PI, Offset);
		Tangent = FProcMeshTangent(PolarPoint(Radius, Pos+PI/2, Offset), false);

		for (int j=0; j<2; j++)
		{
			Normals.Emplace(Normal);
			Tangents.Emplace(Tangent);
			UV0.Emplace(FVector2D(0, 0));
			VertexColors.Emplace(FLinearColor(0.75, 0.75, 0.75, 1.0));
		}
	}
	CylinderMesh->CreateMeshSection_LinearColor(ID, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
}

FVector AFilterCylinder::PolarPoint(const float R, const float Angle, const float Z)
{
	return FVector(cos(Angle) * R, sin(Angle) * R, Z);
}

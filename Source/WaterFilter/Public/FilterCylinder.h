#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "FilterCylinder.generated.h"

UCLASS()
class AFilterCylinder : public AActor
{
	GENERATED_BODY()
	
public:	
	AFilterCylinder();
	virtual void Tick(float DeltaTime) override;
	void CreateCylinder(int ID, float Radius, float Height, float Offset = 0.f, bool VisibleInside = false, int Resolution = 32) const;
	static FVector PolarPoint(float R, float Angle, float Z);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material = nullptr;
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * CylinderMesh;
};

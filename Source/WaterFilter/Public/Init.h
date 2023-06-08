// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ball.h"
#include "FilterCylinder.h"
#include "GameFramework/Actor.h"
#include "Init.generated.h"

UCLASS()
class WATERFILTER_API AInit : public AActor
{
	GENERATED_BODY()
	
public:	
	AInit();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
    float SphereDiameter = 100.f;
    UPROPERTY(EditAnywhere)
    float SphereDiameterTolerance = 50.f;
    UPROPERTY(EditAnywhere)
    float InnerCylinderD = 400.f;
    UPROPERTY(EditAnywhere)
    float OuterCylinderD = 900.f;
    UPROPERTY(EditAnywhere)
    float CylinderH = 800.f;
    UPROPERTY(EditAnywhere)
    float CylindersDelta = 200.f;
	UPROPERTY(EditAnywhere)
	float ScaleFactor = 1000000.f;
    UPROPERTY(EditAnywhere)
    float ParticlesH = 2.f;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Destroyed() override;
	
	float DiameterRandomize(float OffsetD) const {return OffsetD ? FMath::RandRange(-OffsetD, OffsetD) : 0;}
	void GenerateCloud(TArray<ABall*>& Cloud, const float CloudHeight, const float CloudSize, const float BallDiameter, const float OffsetD) const;
	
	// float ScaleFactor = FMath::RandRange(static_cast<int64>(pow(10, 6)), pow(10, 9)); 
	float WaterParticleDiameter = SphereDiameter / ScaleFactor;
	bool FilterReady = false;
	
	UPROPERTY(VisibleAnywhere)
	AFilterCylinder* Filter;
	UPROPERTY(VisibleAnywhere)
	TArray<ABall*> FilterBalls;
	UPROPERTY(VisibleAnywhere)
	TArray<ABall*> WaterBalls;
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	ABall();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material = nullptr;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DefaultBallComponent = nullptr;
	UPROPERTY(VisibleAnywhere)
	TArray<UStaticMeshComponent*> BallComponents;
};

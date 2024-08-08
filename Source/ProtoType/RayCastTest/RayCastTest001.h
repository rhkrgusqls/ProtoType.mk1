// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoType/Sungwoo/MyCharacter.h"
#include "RayCastTest001.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API ARayCastTest001 : public AMyCharacter
{
	GENERATED_BODY()
	virtual void Tick(float DeltaTime)override;

	virtual void GetPoint(FVector2D LU, FVector2D LD, FVector2D RU, FVector2D RD) override;

	UFUNCTION(BlueprintCallable, Category = "View")
	virtual void RayCast(const FVector& StartLocation, const FVector& EndLocation,int32 FloorInfo = 10) override;
	void ChangeMaterialRGB(USkeletalMeshComponent* SkeletalMesh, FName BoneName, FLinearColor NewColor);

	//UFUNCTION(BlueprintCallable, Category = "View")
	//void TempRaycast();
};

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

	void RayCast(UObject* WorldContextObject, const FVector& StartLocation, const FVector& EndLocation);
	void ChangeMaterialRGB(USkeletalMeshComponent* SkeletalMesh, FName BoneName, FLinearColor NewColor);
};

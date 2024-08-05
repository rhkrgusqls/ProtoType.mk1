// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ProtoType/RayCastTest/RayCastTest001.h"

void ARayCastTest001::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    RayCastTest(GetWorld(),,GetActorLocation(),GetActorLocation()+GetActorForwardVector()*2000);
}

void ARayCastTest001::RayCastTest(UObject* WorldContextObject, USkeletalMeshComponent* SkeletalMesh, const FVector& StartLocation, const FVector& EndLocation)
{

    if (!WorldContextObject || !SkeletalMesh) return;

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(SkeletalMesh->GetOwner());


    bool bHit = WorldContextObject->GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);

    if (bHit)
    {

        UE_LOG(LogTemp, Log, TEXT("Hit Location: %s"), *HitResult.ImpactPoint.ToString());


        FVector HitLocation = HitResult.ImpactPoint;
        //FName BoneName = SkeletalMesh->GetBoneName(SkeletalMesh->GetBoneIndexAtLocation(HitLocation));

        FName BoneName = HitResult.BoneName;
        //UE_LOG(LogTemp, Warning, TEXT("Hit Bone: %s"), *BoneName);

        ChangeMaterialRGB(SkeletalMesh, BoneName, FLinearColor::Red);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("No hit detected."));
       
    }
}
void ARayCastTest001::ChangeMaterialRGB(USkeletalMeshComponent* SkeletalMesh, FName BoneName, FLinearColor NewColor)
{
    if (!SkeletalMesh) return;


    int32 BoneIndex = SkeletalMesh->GetBoneIndex(BoneName);
    if (BoneIndex == INDEX_NONE) return;


    TArray<UMaterialInterface*> Materials = SkeletalMesh->GetMaterials();
    for (UMaterialInterface* Material : Materials)
    {
        if (Material)
        {

            UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, SkeletalMesh);
            if (DynamicMaterial)
            {

                DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), NewColor);


                SkeletalMesh->SetMaterial(0, DynamicMaterial);
            }
        }
    }
}
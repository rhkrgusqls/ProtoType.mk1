#include "ProtoType/RayCastTest/RayCastTest001.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"


void ARayCastTest001::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    RayCast(GetWorld(),GetActorLocation(),GetActorLocation()+GetActorForwardVector()*2000);
}

void ARayCastTest001::RayCast(UObject* WorldContextObject, const FVector& StartLocation, const FVector& EndLocation)
{
    if (!WorldContextObject) return;
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    bool bHit = WorldContextObject->GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
    if (bHit)
    {
        FVector HitLocation = HitResult.ImpactPoint;
        FName BoneName = HitResult.BoneName;

        UPrimitiveComponent* HitComponent = HitResult.GetComponent();

        if (HitComponent)
        {
            USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(HitComponent);
            if (SkeletalMeshComponent)
            {
                ChangeMaterialRGB(SkeletalMeshComponent, BoneName, FLinearColor{1.0f,0.0f,0.0f,1.0f});
            }
        }
    }
}
void ARayCastTest001::ChangeMaterialRGB(USkeletalMeshComponent* SkeletalMesh, FName BoneName, FLinearColor NewColor)
{
    if (!SkeletalMesh) return;
    int32 BoneIndex = SkeletalMesh->GetBoneIndex(BoneName);
    if (BoneIndex == INDEX_NONE) return;
    FLinearColor RedColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
    TArray<UMaterialInterface*> Materials = SkeletalMesh->GetMaterials();
    for (UMaterialInterface* Material : Materials)
    {
        if (Material)
        {
            UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, SkeletalMesh);
            if (DynamicMaterial)
            {
                //DynamicMaterial->SetBlendMode(BLEND_Opaque);
                DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), RedColor);
                DynamicMaterial->SetScalarParameterValue(FName("Opacity"), 1.0f);
                SkeletalMesh->SetMaterial(0, DynamicMaterial);
            }
        }
    }
}
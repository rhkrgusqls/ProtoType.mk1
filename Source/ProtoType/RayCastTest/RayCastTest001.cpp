#include "ProtoType/RayCastTest/RayCastTest001.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ProtoType/ClientModule/TCPModule.h"

void ARayCastTest001::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARayCastTest001::GetPoint(FVector2D LU, FVector2D LD, FVector2D RU, FVector2D RD)
{
    //for (const auto& item : MyTCPModule.GetAPData(/* Input std::vector XY = (latitude1, longitude1, latitude2, longitude2, latitude3, longitude3, latitude4, longitude4) 각각 위도 경도를 vector에 추가해서 보내기 */ ))
    //{
    //    /* item.latitude와 item.longitude를 X, Y좌표로 바꾸는 공식*/
    //        RayCast(GetWorld(), FVector(X, Y, 1000000), FVector(X, Y, -100));
    //}
    MyTCPModule.TCPCunnect();
    std::vector<float> lA;
    lA.push_back(36.513564);

    lA.push_back(126.225986);

    lA.push_back(35.513564);

    lA.push_back(128.225986);

    lA.push_back(37.513564);

    lA.push_back(126.225986);

    lA.push_back(35.513564);

    lA.push_back(128.225986);
    for (const auto& item : MyTCPModule.GetAPData(lA)) {
        UE_LOG(LogTemp, Warning, TEXT("%d"), item.ApartIndex);
        RayCast(GetWorld(), FVector((item.latitude- 36.513564) * 100000000, (item.longitude- 127.225986)* 100000000, 10000000000), FVector((item.latitude - 36.513564) * 100000000, (item.longitude - 127.225986) * 100000000, -100));
    }
}

void ARayCastTest001::RayCast(UObject* WorldContextObject, const FVector& StartLocation, const FVector& EndLocation)
{

    DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 50.0f, 0, 100.0f);
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
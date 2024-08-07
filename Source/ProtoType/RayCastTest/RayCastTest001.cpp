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
        //RayCast(FVector((item.latitude- 36.513564) * 100000000, (item.longitude- 127.225986)* 100000000, 10000000000), FVector((item.latitude - 36.513564) * 100000000, (item.longitude - 127.225986) * 100000000, -100));
    }
}

void ARayCastTest001::RayCast(const FVector& StartLocation, const FVector& EndLocation,int32 FloorInfo)
{

    // 머티리얼 속성 설정 (예: 색상)
    FLinearColor NewColor;

    if(FloorInfo<7) NewColor = FLinearColor::Green;
    else if(FloorInfo < 14) NewColor = FLinearColor::Blue; 
    else if (FloorInfo < 20) NewColor = FLinearColor::Red;
    else NewColor = FLinearColor::Black;

    
    UObject* WorldContextObject = GetWorld();
    DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 50.0f, 0, 100.0f);
    if (!WorldContextObject) return;
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    bool bHit = WorldContextObject->GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_GameTraceChannel2 /*ECC_Visibility*/, QueryParams);
    if (bHit)
    {
        UPrimitiveComponent* HitComponent = HitResult.GetComponent();
        AActor* HitActor = HitResult.GetActor();

        if (HitActor)
        {
            UStaticMeshComponent* HitStaticMesh = Cast<UStaticMeshComponent>(HitActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
            if (HitStaticMesh)
            {

                UMaterialInterface* Material = HitStaticMesh->GetMaterial(0);

                if (Material)
                {
                    UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
                    if (DynamicMaterialInstance)
                    {
                        DynamicMaterialInstance->SetVectorParameterValue(FName("Color"), NewColor);

                        // 스태틱 메쉬 컴포넌트에 머티리얼 적용
                        HitStaticMesh->SetMaterial(0, DynamicMaterialInstance);

                        UE_LOG(LogTemp, Warning, TEXT("Material applied successfully."));
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Failed to create dynamic material instance."));
                    }
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No static mesh component found on the hit actor."));
            }
        }


        DrawDebugLine(GetWorld(), HitResult.Location + FVector(0, 0, 10000.0f), HitResult.Location, FColor::Green, false, 5.0f, 0, 1.0f);
        
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
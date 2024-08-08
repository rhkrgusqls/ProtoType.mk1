#include "ProtoType/RayCastTest/RayCastTest001.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "ProtoType/ClientModule/TCPModule.h"

void ARayCastTest001::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

ARayCastTest001::ARayCastTest001()
{
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Sungwoo/M_DynamicInst.M_DynamicInst'"));

    if (MaterialFinder.Succeeded())
    {
        InstMaterial = MaterialFinder.Object;
    }
}

void ARayCastTest001::GetPoint(FVector2D LU, FVector2D LD, FVector2D RU, FVector2D RD)
{
    //for (const auto& item : MyTCPModule.GetAPData(/* Input std::vector XY = (latitude1, longitude1, latitude2, longitude2, latitude3, longitude3, latitude4, longitude4) ���� ���� �浵�� vector�� �߰��ؼ� ������ */ ))
    //{
    //    /* item.latitude�� item.longitude�� X, Y��ǥ�� �ٲٴ� ����*/
    //        RayCast(GetWorld(), FVector(X, Y, 1000000), FVector(X, Y, -100));
    //}

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

    // ��Ƽ���� �Ӽ� ���� (��: ����)
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
    bool bHit = WorldContextObject->GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
    if (bHit)
    {
        UPrimitiveComponent* HitComponent = HitResult.GetComponent();
        AActor* HitActor = HitResult.GetActor();

        if (HitActor)
        {
            if (HitComponent->GetCollisionObjectType() == ECC_GameTraceChannel1)//�ǹ� ������
            {
                ChangeBuildingMaterial(HitResult, NewColor);
            }
            else//�ٴڸ�����
            {
                float SphereRadius = 1500.0f;
                FHitResult SphereHitResult;
                FCollisionQueryParams SphereQueryParams;
                SphereQueryParams.AddIgnoredActor(this);
                bool bResult = GetWorld()->SweepSingleByChannel(
                    SphereHitResult,
                    HitResult.ImpactPoint,
                    HitResult.ImpactPoint + FVector(0, 0, 100.0f),
                    FQuat::Identity,
                    ECollisionChannel::ECC_GameTraceChannel3,
                    FCollisionShape::MakeSphere(SphereRadius),
                    SphereQueryParams

                );

                if (bResult) {
                    HitComponent = SphereHitResult.GetComponent();
                    if (HitComponent->GetCollisionObjectType() == ECC_GameTraceChannel1)
                    {
                        ChangeBuildingMaterial(SphereHitResult, NewColor);
                    }




#if ENABLE_DRAW_DEBUG//����� ��忡���� ����� ĸ�� �׸�����
                    FVector TraceVec = FVector(0, 0, 100.f);
                    FVector Center = HitResult.ImpactPoint + TraceVec * 0.5f;
                    float HalfHeight = 50.f;
                    FColor DrawColor = (HitComponent->GetCollisionObjectType() == ECC_GameTraceChannel1) ? FColor::Green : FColor::Red;
                    float DebugLifeTime = 50.0f;

                    DrawDebugCapsule(GetWorld(),
                        Center,
                        HalfHeight,
                        SphereRadius,//radius
                        FQuat::Identity,
                        DrawColor,
                        false,
                        DebugLifeTime
                    );

#endif // ENABLE_DRAW_DEBUG//����� ��忡���� ����� ĸ�� �׸�����
                    if (bResult)
                    {

                    }
                }
            }
        }


        DrawDebugLine(GetWorld(), HitResult.Location + FVector(0, 0, 10000.0f), HitResult.Location, FColor::Green, false, 5.0f, 0, 1.0f);
        
    }
}
void ARayCastTest001::ChangeBuildingMaterial(FHitResult& HitResult, FLinearColor InNewColor)
{
    UPrimitiveComponent* HitComponent = HitResult.GetComponent();
    AActor* HitActor = HitResult.GetActor();
    if (HitActor)
    {
        if (HitComponent->GetCollisionObjectType() == ECC_GameTraceChannel1)
        {
            UStaticMeshComponent* HitStaticMesh = Cast<UStaticMeshComponent>(HitActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

            if (HitStaticMesh)
            {

                if (InstMaterial)
                {
                    UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(InstMaterial, this);
                    if (DynamicMaterialInstance)
                    {
                        DynamicMaterialInstance->SetVectorParameterValue(FName("Color"), InNewColor);

                        // ����ƽ �޽� ������Ʈ�� ��Ƽ���� ����
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
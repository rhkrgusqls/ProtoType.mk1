// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtoType/TCPTest/TCPTestActor.h"
#include "ProtoType/ClientModule/TCPModule.h"
#include <vector>
#include "Engine/Engine.h"
#include <string>
// Sets default values
ATCPTestActor::ATCPTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATCPTestActor::BeginPlay()
{
	Super::BeginPlay();
	TCPModule& MyTCPModule = TCPModule::GetInstance();
	MyTCPModule.TCPCunnect();
	for (const auto& item : MyTCPModule.GetAPData()) {
		UE_LOG(LogTemp, Warning, TEXT("%d"), item.ApartIndex);

	}
}

// Called every frame
void ATCPTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


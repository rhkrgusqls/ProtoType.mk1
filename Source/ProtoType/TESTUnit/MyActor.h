// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include<stdio.h>
#include<iostream>
#include<Winsock2.h>

#include "MyActor.generated.h"



UCLASS()
class PROTOTYPE_API AMyActor : public AActor
{
	GENERATED_BODY()
	
private:
	struct cal_data {
		int left_num;
		int right_num;
		char op;
		int result;
		int error;
	};
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int TCPModule();
	void HandleError(const char* cause);

	//cal_data sdata;
	SOCKET s;
	WSADATA wsaData;
	SOCKADDR_IN addr;
	char buffer[256];
	int len=0;
	int sbyte = 0;
	int rbyte=0;
	struct cal_data sdata;

};



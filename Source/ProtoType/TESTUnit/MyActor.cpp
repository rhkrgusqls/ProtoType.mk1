// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtoType/TESTUnit/MyActor.h"
#pragma warning(disable:4996)

#include<stdio.h>
#include<iostream>
#include<Winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT	4960
#define IP		"203.249.91.170"

DEFINE_LOG_CATEGORY_STATIC(MyLogCategory, Warning, All);

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(MyLogCategory, Warning, TEXT("MyIntValue: %d"), TCPModule());
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Log the current value every fram
}

int AMyActor::TCPModule()
{
	//ZeroMemory((void*)&sdata, sizeof(sdata));
	sdata.left_num = 100;
	sdata.right_num = 50;
	sdata.op = '+';

	if (::WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		HandleError("WSAStartup");

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
		HandleError("socket");

	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(IP);
	addr.sin_port = ::htons(PORT);

	if (::connect(s, (const sockaddr*)&addr, sizeof(addr)) == INVALID_SOCKET)
		HandleError("connet");

	len = sizeof(sdata);
	sdata.left_num = htonl(sdata.left_num);
	sdata.right_num = htonl(sdata.right_num);
	sbyte = ::send(s, (char*)&sdata, len, 0);
	if (sbyte != len)
		HandleError("Send");

	rbyte = recv(s, (char*)&sdata, len, 0);
	if (rbyte != len)
		HandleError("Recv");

	if (ntohs(sdata.error != 0))

		::closesocket(s);
	WSACleanup();
	return ntohl(sdata.result);
	return 0;
}

void AMyActor::HandleError(const char* cause)
{
	int errCode = ::WSAGetLastError();
}

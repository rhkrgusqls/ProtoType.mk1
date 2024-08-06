#include "ProtoType/ClientModule/TCPModule.h"

#pragma warning(disable:4996)

#include<stdio.h>
#include<iostream>
#include<Winsock2.h>
#include<vector>

#include <thread>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

#define PORT	4960
#define IP		"203.249.91.170"

DEFINE_LOG_CATEGORY_STATIC(MyLogCategory, Warning, All);


std::vector<APData> TCPModule::GetAPData(std::vector<float> Elemental)
{
	SSelectorType Selector;
	Selector.Type = 1;

	Selector.Elemental[0] = Elemental[0];
	Selector.Elemental[1] = Elemental[1];

	Selector.Elemental[2] = Elemental[2];
	Selector.Elemental[3] = Elemental[3];

	Selector.Elemental[4] = Elemental[4];
	Selector.Elemental[5] = Elemental[5];

	Selector.Elemental[6] = Elemental[6];
	Selector.Elemental[7] = Elemental[7];

	Selector.MaxElIndex = 8;

	std::vector<APData> VAP;
	len = sizeof(Selector);
	APData AP;
	int APSize;
	//--Recive001--Complite
	send(s, (char*)&Selector.Type, len, 0);
	
	//--Recive002
	send(s, (char*)&Selector.MaxElIndex, sizeof(Selector.MaxElIndex), 0);
	for (int32 i = 0; i < Selector.MaxElIndex; ++i)
	{        //--Recive003
		send(s, (char*)&Selector.Elemental[i], sizeof(Selector.Elemental[i]), 0);
	}


	recv(s, (char*)&APSize, sizeof(APSize), 0);
	UE_LOG(LogTemp, Warning, TEXT("%d"), APSize);
	for (int32 i = 0; i < APSize; ++i)
	{
		int len202=0;
		//--Recive001
		recv(s, (char*)&len202, sizeof(len202), 0);
		//--Recive002
		recv(s, (char*)&AP, len202, 0);
		VAP.push_back(AP);
	}
	SAPData = VAP;
	return VAP;
}



void TCPModule::TCPCunnect()
{
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
	return;
}

void TCPModule::HandleError(const char* cause)
{
	int errCode = ::WSAGetLastError();
}
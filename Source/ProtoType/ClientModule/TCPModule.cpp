#include "ProtoType/ClientModule/TCPModule.h"

#pragma warning(disable:4996)

#include<stdio.h>
#include<iostream>
#include<Winsock2.h>
#include<vector>
#pragma comment(lib, "ws2_32.lib")

#define PORT	4960
#define IP		"203.249.91.170"

DEFINE_LOG_CATEGORY_STATIC(MyLogCategory, Warning, All);


std::vector<APData> TCPModule::GetAPData()
{
	SSelectorType Selector;
	Selector.Type = 1;
	Selector.Elemental.push_back(1);

	Selector.Elemental.push_back(2);

	Selector.Elemental.push_back(3);

	Selector.Elemental.push_back(4);
	std::vector<APData> VAP;
	len = sizeof(Selector);
	APData AP;
	int APSize;
	send(s, (char*)&Selector, len, 0);
	
	recv(s, (char*)&APSize, sizeof(APSize), 0);
	UE_LOG(LogTemp, Warning, TEXT("%d"), APSize);
	for (int32 i = 0; i < APSize - 1; ++i)
	{
		int len202=0;
		recv(s, (char*)&len202, sizeof(len202), 0);
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
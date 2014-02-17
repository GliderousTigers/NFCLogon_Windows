#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <sstream>

#include "UDPComm.h"
#include "UDPCommResponse.h"
#include "ByteUtils.h"

using namespace std;

#pragma comment(lib, "Ws2_32.lib")

UDPComm::UDPComm(int port, char* broadcastData) : Comm()
{
	Journal::log("UDPCommCreate\n");
	this->port = port;
	
	if (SUCCEEDED(this->initWinSock()))
	{
		Journal::log("Winsock initiated\n");
		if (SUCCEEDED(this->createSocket(port)))
		{
			Journal::log("Socket created\n");
			this->sendData = broadcastData;
		}
		else
		{
			Journal::log("Error on Socket creation\n");
		}
	}
	else
	{
		Journal::log("Error on Winsock initialization\n");
	}
}

UDPComm::~UDPComm()
{
}

HRESULT UDPComm::initWinSock()
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return HRESULT_FROM_WIN32(WSAGetLastError());
	}
	return S_OK;
}

HRESULT UDPComm::createSocket(int port)
{
	this->s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (this->s == SOCKET_ERROR)
	{
		return HRESULT_FROM_WIN32(WSAGetLastError());
	}

	return this->bindSocket(port);
}

HRESULT UDPComm::bindSocket(int port)
{
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	int result = ::bind(this->s, (SOCKADDR*)&server, sizeof(server));
	if (this->s == INVALID_SOCKET || result == SOCKET_ERROR)
	{
		closesocket(this->s);
		return HRESULT_FROM_WIN32(WSAGetLastError());
	}
	return S_OK;
}

void UDPComm::run()
{
	Journal::log(("thread running\n"));
	struct sockaddr_in fromAddr;
	int fromLength = sizeof(fromAddr);
	this->receivedData = new char[BUFFER_LENGTH];

	//while (!this->isClosed())
	//{
		int retour = recvfrom(this->s, this->receivedData, BUFFER_LENGTH, 0, (struct sockaddr *)&fromAddr, &fromLength);
		//if (retour == 0)
		//{
		//	continue;
		//}
		//else if (retour > 0)
		if (retour > 0)
		{
			char* cleanedData = ByteUtils::subArray(this->receivedData, 0, retour);
			Journal::log(("received" + string(cleanedData) + "\n"));
			this->notifyListeners(UDPCommResponse(&this->s, this, cleanedData, (SOCKADDR*)&fromAddr));
		}
		else
		{
			stringstream ss;
			ss << WSAGetLastError();
			Journal::log(("error winsock:" + ss.str() + "\n"));
			return;
		}
	//}
	Journal::log("thread endded\n");
	//::closesocket(this->s);
}


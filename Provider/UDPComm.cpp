#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <sstream>

#include "UDPComm.h"

using namespace std;

#pragma comment(lib, "Ws2_32.lib")

UDPComm::UDPComm(int port, char* broadcastData)
{
	this->port = port;

	this->broadcast = (broadcastData != NULL);
	
	if (SUCCEEDED(this->initWinSock()))
	{
		if (this->broadcast)
		{
			if (SUCCEEDED(this->createSocket(port)))
			{
				this->sendData = broadcastData;
			}
		}
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
	//stringstream ss;
	//ss << port;

	////get host (us) socket info
	//if (getaddrinfo(NULL, ss.str().c_str(), hints, &(this->hostInfos)) != 0)
	//{
	//	freeaddrinfo(this->hostInfos);
	//	return HRESULT_FROM_WIN32(WSAGetLastError());
	//}
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
	hostent* localHost;
	localHost = gethostbyname("");
	char* localIP = inet_ntoa(*(struct in_addr *)*localHost->h_addr_list);

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(localIP);

	bind(this->s, (SOCKADDR*) &server, sizeof(server));
	if (this->s == SOCKET_ERROR)
	{
		closesocket(this->s);
		return HRESULT_FROM_WIN32(WSAGetLastError());
	}
	return S_OK;
}
void UDPComm::stopBroadcasting()
{
	this->broadcast = FALSE;
}

void UDPComm::run()
{
	struct sockaddr_in fromAddr;
	fromAddr.sin_port = htons(this->port);
	while (this->isClosed())
	{
		if (this->broadcast)
		{
			//determine if we receive or send first
			if (sendto(this->s, this->sendData, strlen(this->sendData), 0, (SOCKADDR*)&fromAddr, sizeof(fromAddr)) == SOCKET_ERROR)
			{
			}
		}
	}
}

//HRESULT WirelessListener::Receive()
//{
//	char dataReceived[WirelessListener::BUFFER_LENGTH];
//	sockaddr_in tempAddr;
//	int fromLength = sizeof(tempAddr);
//
//	int receiverResult;
//
//	do 
//	{
//		receiverResult = recvfrom(this->s, dataReceived, WirelessListener::BUFFER_LENGTH, 0, (struct sockaddr *) &tempAddr, &fromLength);
//		if (receiverResult > 0)
//		{
//			this->fromAddr = tempAddr;
//			this->handleReceivedData(dataReceived, receiverResult);
//		}
//		
//	} while(receiverResult > 0);
//
//	std::printf("exiting\n");
//	return S_OK;
//}

//void WirelessListener::handleReceivedData(char* buffer, int dataLenght)
//{
//	//analyze data and call function corresponding to the data sended.
//	string data(buffer);
//	data = data.substr(0, dataLenght);
//	printf("\n");
//	if (data == "unlock")
//	{
//		this->Send(data.data());
//		//this->provider->QueryInterface(); //start the authentification process
//	}
//}

//HRESULT WirelessListener::Send(const char* data)
//{
//	if (sendto(this->s, data, strlen(data), 0, (struct sockaddr*) &this->fromAddr, sizeof(this->fromAddr)) == SOCKET_ERROR) 
//	{
//		//throw ICredentialListenerException("Socket Error when sending data");
//		closesocket(this->s);
//		this->s = INVALID_SOCKET; //so that it is rebinded on next receive.
//        return HRESULT_FROM_WIN32(WSAGetLastError());
//    }
//	return S_OK;
//}

#include "UDPCommResponse.h"

UDPCommResponse::UDPCommResponse(SOCKET* s, UDPComm* comm, char* data, sockaddr* from) : CommResponse(data)
{
	this->s = s;
	this->comm = comm;
	this->senderInfos = from;
}

void UDPCommResponse::reply(char* data)
{
	this->comm->stopBroadcasting();
	sendto(*this->s, this->data, strlen(this->data), 0, (SOCKADDR*)&this->senderInfos, sizeof(this->senderInfos));
}
/** @file UDPCommResponse.h
Class to represent a response data received over UDP.

@author Alexandre Cormier
@author Jerome St-Hilaire
@date 8/2/2014
@version 0.1
*/

#pragma once

#include "CommResponse.h"
#include "UDPComm.h"

class UDPCommResponse : public CommResponse
{
public:
	/**
	* Initializes class attributes with the parameters.
	* @param s The socket to be use for the reply
	* @param comm Pointer to the UDPComm object that received the data.
	* @param data Is passed to super class constructor.
	* @param from Ip Adress of the sender.
	*/
	UDPCommResponse(SOCKET* s, UDPComm* comm, char* data, sockaddr* from);
	~UDPCommResponse();

	/**
	* @copydoc CommResponse::reply()
	*/
	virtual void reply(char* data);
private:
	/**
	* Pointer to the UDPComm object that received the data.
	*/
	UDPComm* comm;

	/**
	* The address from which the data was received
	*/
	struct sockaddr* senderInfos;

	/**
	* The socket on which the data was received.
	*/
	SOCKET* s;
};
/** @file UDPComm.h
Class for UDP communication between the PC and the Device.

@author Alexandre Cormier
@author Jerome St-Hilaire
@date 8/2/2014
@version 0.1
*/

#pragma once

#include "Comm.h"

#define BUFFER_LENGTH 512

class UDPComm : public Comm
{
public:	
	/**
	* create a new UDPComm, initializes a socket and binds it
	* to the specified port.
	* @param port The port on which the socket will be bound
	* @param broadcastData The data we wish to broadcast with it. 
	* If null the UDPComm will wait to receive data first.
	*/
	UDPComm(int port, char* broadcastData = NULL);
	~UDPComm();

protected:
	/**
	* @copydoc Comm::run()
	*/
	virtual void run();
private:

	/**
	* Creates an UDP socket.
	* @return S_OK if it succeeds else, an error code.
	*/
	HRESULT createSocket(int port);

	/*
	* This fonction inits the Winsock librairy
	* @return S_OK if it succeeds else, an error code.
	*/
	HRESULT initWinSock();

	/**
	* Binds the SOCKET s to a port on the localHost
	* @return S_OK if it succeeds else, an error code.
	*/
	HRESULT bindSocket(int port);

	/**
	* socket on which to send and receive data
	*/
	SOCKET s;

	/**
	* buffer for the data we receive
	*/
	char* receivedData;

	/**
	* buffer for the data we send
	*/
	char* sendData;

	/**
	* Port on wich we communicate
	*/
	int port;

};
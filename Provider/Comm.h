/** @file Comm.h
Class to represent the communication between the Android device
and the PC. A subclass should be created for each communication mean.

@author Alexandre Cormier
@author Jerome St-Hilaire
@date 8/2/2014
@version 0.1
*/

#pragma once

#include <vector>
#include <Windows.h>
#include <thread>

#include "CommListener.h"
#include "CommResponse.h"

using namespace std;

class Comm
{
public: 
	/**
	* Constructs a new Comm object.
	*/
	Comm();
	~Comm() {}

	/**
	* Starts the communication. The  Comm::run() method is called from a new thread.
	*/
	void start();
	
	/**
	* Registers a listener with this communication.
	* @param listener the listener to register.
	*/
	void addListener(CommListener* listener);
	
	/**
	* Unregisters a listener from this communication.
	* @param listener the listener to unregister.
	* @return whether or not the listener was unregistered.
	*/
	void removeListener(CommListener* listener);
	
	/**
	* Gets whether or not the communication was closed.
	* @return whether or not the communication was closed.
	*/
	bool isClosed();
	
	/**
	* Closes this communication and waits for the worker thread to exit.
	*/
	void close();
protected:

	/**
	* Called from a worker thread when the communication is started. Subclasses should
	* loop as long as Comm::isClosed() returns FALSE.
	*/
	virtual void run() = 0;
	
	/**
	* Notifies listeners of the reception of a packet.
	* @param response object containing the received packet and a mean to reply.
	*/
	void notifyListeners(CommResponse& response);
private:
	/**
	* list of listeners waiting for the reception of packets
	*/
	vector<CommListener*> listeners;

	/**
	* whether or not the communication was closed
	*/
	bool closed;

	/**
	* thread in which the communication is run
	*/
	thread hThread;
};
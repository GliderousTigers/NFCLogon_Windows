/** @file CommListener.h

Interface for a communication listener, to get the data received.

@author Alexandre Cormier
@author Jerome St-Hilaire
@date 8/2/2014
@version 0.1
*/

#pragma once

#include "CommResponse.h"

using namespace std;

class CommListener
{
public:
	/**
	* Called when a packet is received.
	* @param response object containing the received packet and a mean to reply.
	*/
	virtual void onDataRecieved(CommResponse& response) = 0;
};
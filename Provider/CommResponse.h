/** @file CommResponse.h
Class to encapsulate the data received from a communication and provide
some methods that will be useful when receiving data, namely to reply

@author Alexandre Cormier
@author Jerome St-Hilaire
@date 8/2/2014
@version 0.1
*/

#pragma once

class CommResponse
{
public:
	/**
	* Constructs a new response.
	* @param data the data recived.
	*/
	CommResponse(char* data)
	{
		this->data = data;
	}

	/**
	* Replies to the client who sent the data.
	* @param data data to send.
	*/
	virtual void reply(char* data) = 0;
protected:
	/**
	* The data received.
	*/
	const char* data;
};
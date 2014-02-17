/** @file AbstractPacket.h
Base class for different packets passing from the Android device
to the PC and vice-versa during pairing and the unlocking handshake.
All subclasses must have a default constructor taking no parameter.

@author Alexandre Cormier
@author Jerome St-Hilaire
@date 8/2/2014
@version 0.1
*/
#pragma once

#include <map>
#include <vector>
#include <string>

#include "PacketField.h"

using namespace std;

class AbstractPacket
{
public:
	
	/**
	* Gets a packet object from the data received.
	* @param data raw data received from the PC.
	* @return an AbstractPacket* constructed with the data received.
	*/
	static AbstractPacket* get(unsigned char* data);

	/**
	* Gets the raw data of this packet to send to the PC.
	* @return this packet's data.
	*/
	unsigned char* getBytes();
protected:

	/**
	* Constructs an empty packet with the specified type name.
	* @param packetType the type of this packet. Each subclass must have
	*                   a unique name and pass it here.
	*/
	AbstractPacket(unsigned char* packetType)
	{
		this->packetType = packetType;
	}
	virtual ~AbstractPacket() {};

	/**
	* Called when receiving a packet and creating it with {@link #get(byte[])}, after
	* it has been parsed and fields are available from the {@link #getField(int)} method.
	* @throws InvalidPacketException if the received packet's format is incorrect.
	*/
	virtual void onParseFinished() = 0;

	/**
	* Gets the fields to add to this packet when constructing it to send.
	* @return this packets' fields.
	*/
	virtual vector<PacketField> getFields() = 0;

	/**
	* Gets a field from its position in the packet.
	* @param index position in the packet of the field to get.
	* @return the field at the specified index.
	*/
	PacketField getField(int index);
private:
	/**
	* Create a packet from the type passed in parameter
	* @param type The type of packet
	* @return A ppointer to an instance of a subclass of AbstractPacket
	*/
	static AbstractPacket* createInstance(unsigned char* type);

	/**
	* Parse the data received into fields
	*/
	void parse(unsigned char* data);

	/**
	* Fields contained in this packet
	*/
	vector<PacketField> fields;

	/**
	* Pakcet type name
	*/
	unsigned char* packetType;
};
/** @file PacketField.h
Wrapper containing a byte array of data.

@author Alexandre Cormier
@author Jerome St-Hilaire
@date 8/2/2014
@version 0.1
*/

#pragma once

class  PacketField
{
public:
	/**
	* Constructs a new field with the data provided.
	* @param data the field's data.
	*/
	PacketField(unsigned char* data);

	/**
     * Constructs a new field object from a received packet.
     * @param packetData the packet's data.
     * @param offset the offset at which the desired field is located.
     */
	PacketField(unsigned char* data, int offset);
	~PacketField();
	/**
	* Gets this field as bytes, as it must be added to a packet.
	* @return this field as bytes.
	*/
	unsigned char* getBytes();

	/**
	* Gets the total length of this field, including the four
	* bytes used to store the data's length.
	* @return the total length of this field.
	*/
	int getLenght();

	/**
	* Gets this field's data.
	* @return this field's data.
	*/
	unsigned char* getData();
private:

	/**
	* the field's data
	*/
	unsigned char* data;
};
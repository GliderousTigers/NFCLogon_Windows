#include "AbstractPacket.h"
#include "RSAPublicKeyPacket.h"
#include <string>
#include "ByteUtils.h"
#include "CharUtils.h"
#include "RawPacket.h"

AbstractPacket* AbstractPacket::get(unsigned char* data)
{
	PacketField packetTypeField(data, 0);
	AbstractPacket* packet = AbstractPacket::createInstance(packetTypeField.getData());
	unsigned char* temp = new unsigned char[];
	std::memcpy(temp, data + packetTypeField.getLenght(), sizeof(data) - 1);
	packet->parse(temp);

	return packet;
}

unsigned char* AbstractPacket::getBytes()
{
	
	unsigned char* data = PacketField(this->packetType).getBytes();

	for each (PacketField field in this->fields)
	{
		ByteUtils::concat(data, field.getBytes());
	}

	return data;
}

PacketField AbstractPacket::getField(int index)
{
	return this->fields.at(index);
}

void AbstractPacket::parse(unsigned char* data)
{
	this->fields.clear();
	int offset = 0;

	while (offset < sizeof(data))
	{
		PacketField field(data, offset);
		this->fields.push_back(field);
		offset += field.getLenght();
	}
}

AbstractPacket* AbstractPacket::createInstance(unsigned char* type)
{
	if (type == RSAPublicKeyPacket::PACKET_TYPE)
	{
		return new RSAPublicKeyPacket();
	}
	else if (type == RawPacket::PACKET_TYPE)
	{
		return new RawPacket();
	}
}
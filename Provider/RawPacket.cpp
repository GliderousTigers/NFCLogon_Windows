#include "RawPacket.h"
#include "helpers.h"

unsigned char RawPacket::PACKET_TYPE[1] = {""};

void RawPacket::onParseFinished() 
{
	unsigned char* message = this->getField(0).getData();
	this->message = message;
}

vector<PacketField> RawPacket::getFields()
{
	std::vector<PacketField> fields;
	fields.push_back(PacketField((this->message)));
	return fields;
}
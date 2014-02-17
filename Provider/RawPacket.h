#pragma once
#include "AbstractPacket.h"

#include <string>

class RawPacket : public AbstractPacket
{
public:
	RawPacket() : AbstractPacket(PACKET_TYPE) {}
	~RawPacket() {}
	static unsigned char PACKET_TYPE[1];
protected:
	virtual void onParseFinished();
	virtual vector<PacketField> getFields();
	
	unsigned char* message;
};
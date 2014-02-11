#include "PacketField.h"
#include "ByteUtils.h"

PacketField::PacketField(unsigned char* data)
{
	this->data = data;
}
PacketField::PacketField(unsigned char* data, int offset)
{
	int lenght = ByteUtils::getInt(ByteUtils::subArray(data, offset, 4));
	this->data = ByteUtils::subArray(data, offset + 4, lenght);
}

PacketField::~PacketField()
{
	delete[] this->data;
}
unsigned char* PacketField::getBytes()
{
	return ByteUtils::concat(ByteUtils::toBytes(sizeof(this->data)), this->data);
}
int PacketField::getLenght()
{
	return sizeof(this->data) + 4;
}
unsigned char* PacketField::getData()
{
	return this->data;
}
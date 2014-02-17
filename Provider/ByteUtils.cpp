#include "ByteUtils.h"
#include <string>
#include <stdio.h>

unsigned char* ByteUtils::toBytes(int i)
{
	unsigned char* bytes = new unsigned char[4];
	for (int j = 0; j < 4; j++)
	{
		bytes[j] = (unsigned char)(i >> (8 * (3 - j)));
	}
	return bytes;
}
const int ByteUtils::getInt(unsigned char* bytes)
{
	int i = 0;
	for (int j = 0; j < 4; j++)
	{
		i |= (bytes[j] & 0xFF) << (8 * (3 - j));
	}
	return i;
}
unsigned char* ByteUtils::subArray(unsigned char* data, int offset, int lenght)
{
	unsigned char* temp = new unsigned char[lenght];
	std::memcpy(temp, data + offset, lenght);
	return temp;
}

unsigned char* ByteUtils::concat(unsigned char* first, ...)
{
	va_list others;
	va_start(others, first);
	int length = sizeof(first);

	for (int i = 0; i < sizeof(others); i++)
	{
		length = sizeof(va_arg(others, unsigned char*));
	}

	unsigned char* temp = new unsigned char[length];

	std::memcpy(temp, first, length);

	int offset = sizeof(first);
	for (int i = 0; i < sizeof(others); i++)
	{
		unsigned char* other = va_arg(others, unsigned char*);
		std::memcpy(temp + offset, other, sizeof(other));
		offset += sizeof(other);
	}
	va_end(others);
	return temp;
}

char* ByteUtils::subArray(char* data, int offset, int length)
{
	char* subArray = new char[length + 1];
	std::memcpy(subArray, data + offset, length);
	subArray[length] = '\0';
	std::printf(subArray);

	return subArray;
}
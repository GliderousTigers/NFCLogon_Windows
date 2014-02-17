#include "CommResponse.h"

CommResponse::CommResponse(char* data)
{
	this->data = data;
}

const char* CommResponse::getData()
{
	return this->data;
}
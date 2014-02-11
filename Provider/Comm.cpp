#include <algorithm>

#include "Comm.h"

Comm::Comm()
{
	this->closed = FALSE;
}

void Comm::start()
{
	this->hThread = std::thread(&Comm::run, this);
}

void Comm::addListener(CommListener* listener)
{
	this->listeners.push_back(listener);
}

void Comm::removeListener(CommListener* listener)
{
	vector<CommListener*>::iterator iter = std::find(this->listeners.begin(), this->listeners.end(), listener);
	if (iter != this->listeners.end())
	{
		this->listeners.erase(iter);
	}
}

void Comm::notifyListeners(CommResponse& response)
{
	for each (CommListener* listener in this->listeners)
	{
		listener->onDataRecieved(response);
	}
}

bool Comm::isClosed()
{
	return this->closed;
}

void Comm::close()
{
	this->closed = TRUE;

	this->hThread.join();
}
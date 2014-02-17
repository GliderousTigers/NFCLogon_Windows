#include <algorithm>

#include "Comm.h"

Comm::Comm()
{
	this->closed = FALSE;
}

void Comm::start()
{
	Journal::log(("thread start\n"));
	this->hThread = std::thread(&Comm::run, this);
}

void Comm::addListener(CommListener* listener)
{
	Journal::log(("addListeners\n"));
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
	Journal::log(("NotifyListeners\n"));
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
	Journal::log(("closing thread\n"));
	this->closed = TRUE;

	this->hThread.join();
}
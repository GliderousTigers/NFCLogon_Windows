#include "Journal.h"
#include <fstream>
#include <string>
#include <iostream>


void Journal::log(string message)
{
	try
	{
		ofstream out("C:\\Users\\Jerome\\Desktop\\log.txt", ios::app);
		out << message;
		out.close();
	}
	catch (...)
	{
		return;
	}

}
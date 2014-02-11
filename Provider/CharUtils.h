/**
* @file CharUtils.h
* Utility class to perform operations on strings and char*s
* @author Jerome St-Hilaire
* @date 8/2/2014
* @version 0.1
*/


#pragma once

#include <string>

class CharUtils
{
public:
	/**
	* Converts a const char* to char*
	* @param arr The const char* to be converted
	* @return The const char* in char*.
	*/
	static char* unConst(const char* arr)
	{
		char* temp = new char[sizeof(arr)];
		strcpy(temp, arr);

		return temp;
	}
private:
	/**
	* This class can't be instanciated. It is a Utility class
	*/
	CharUtils();
	~CharUtils();
};
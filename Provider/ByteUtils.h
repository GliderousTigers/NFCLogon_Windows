#pragma once

/**
* @file ByteUtils.h
* Utility class to perform operations on bytes and byte arrays.
* @author Alexandre Cormier
* @author Jerome St-Hilaire
* @date 8/2/2014
*/


#include <cstdarg>

class ByteUtils
{
public:

	/**
	* Converts an int to a byte array.
	* @param i the int to convert.
	* @return the int as a byte array.
	*/
	static unsigned char* toBytes(int i);

	/**
	* Converts a byte array to an int.
	* @param bytes the byte array to convert.
	* @return the byte array as an int.
	*/
	static const int getInt(unsigned char* bytes);

	/**
	* Gets a part of a byte array.
	* @param array the array to get a part of.
	* @param offset the start index of the sub-array.
	* @param length the length of the sub-array.
	* @return the sub-array starting at offset (inclusive) and ending at
	*         offset + length (exclusive)
	*/
	static unsigned char* subArray(unsigned char* data, int offset, int lenght);

	static char* subArray(char* data, int offset, int length);

	/**
	* Concatenates byte arrays.
	* @param first the first array.
	* @param others the other arrays to add to the first.
	* @return a single array that contains those passed as parameters
	*/
	static unsigned char* concat(unsigned char* first, ...);
private:
	/**
	* This class can't be instanciated. It is a Utility class
	*/
	ByteUtils();
	~ByteUtils();
};
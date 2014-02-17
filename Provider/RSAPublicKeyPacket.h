/** @file RSAPublicKeyPacket.h
Implementation of a packet (received or sent)
containing information about a RSA public Key

@author Alexandre Cormier
@author Jerome St-Hilaire
@date 8/2/2014
@version 0.1
*/

#pragma once
#include "AbstractPacket.h"

#include <string>
#include "Crypto.h"

class RSAPublicKeyPacket : public AbstractPacket
{
public:
	/** default constructor needed for AbstractPacket.get() */
	RSAPublicKeyPacket() : AbstractPacket(PACKET_TYPE) {}

	/**
     * Construct a new {@link java.security.interfaces.RSAPublicKey} from the public
     * key to send.
     * @param key the public key to send.
     */
	RSAPublicKeyPacket(RSA* key) : AbstractPacket(PACKET_TYPE) 
	{
		this->key = key;
	}
	~RSAPublicKeyPacket() {}
	static unsigned char PACKET_TYPE[2]; /**< The name of this packet type.*/

	/**
	* Gets the public key encapsulated in this packet.
	* @return the public key encapsulated in this packet.
	*/
	RSA* getPublicKey();
protected:
	/**
	*  @copydoc AbstractPacket::onParseFinished()
	*/
	virtual void onParseFinished();

	/**
	*  @copydoc AbstractPacket::getFields()
	*/
	virtual vector<PacketField> getFields();
private:
	RSA* key; /**< the RSA public key itself*/
};
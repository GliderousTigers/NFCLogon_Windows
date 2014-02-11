#include "RSAPublicKeyPacket.h"

#include <vector>
#include "CharUtils.h"

unsigned char RSAPublicKeyPacket::PACKET_TYPE[2] = { 0x50, 0x4B };

vector<PacketField> RSAPublicKeyPacket::getFields()
{
	std::vector<PacketField> fields;
	unsigned char* modulus;
	unsigned char* exponent;
	BN_bn2bin(this->key->n, modulus);
	BN_bn2bin(this->key->n, exponent);
	fields.push_back(PacketField(modulus));
	fields.push_back(PacketField(exponent));

	return fields;
}

void RSAPublicKeyPacket::onParseFinished()
{
	unsigned char* mod = this->getField(0).getData();
	BIGNUM modulus; 
	*BN_bin2bn(mod, sizeof(mod), &modulus);

	unsigned char* exp = this->getField(1).getData();
	BIGNUM exponent;
	*BN_bin2bn(exp, sizeof(exp), &exponent);

	this->key = Crypto::getPublicKey(modulus, exponent);
}

RSA* RSAPublicKeyPacket::getPublicKey()
{
	return this->key;
}
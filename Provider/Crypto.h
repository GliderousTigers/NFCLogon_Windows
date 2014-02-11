#pragma once
#include <openssl\pem.h>
#include <openssl\bn.h>
#include <openssl\rsa.h>
#include <openssl\aes.h>
#include <string>

#define DEFAULT_RSA_EXP 65537
#define DEFAULT_RSA_SIZE 4096
#define AES_BLOCK_SIZE 128

using namespace std;

class Crypto
{
public:
	static RSA* getPublicKey(BIGNUM modulus, BIGNUM exponent);
	static RSA* GenerateKeyPair();
	static unsigned char* aes_encrypt(unsigned char* key, unsigned char* IV, unsigned char* data, int* len);
	static unsigned char* aes_decrypt(unsigned char* key, unsigned char* IV, unsigned char* ciphertext, int* len);
	static void aes_init(EVP_CIPHER_CTX* e);

	static unsigned char* generateSecretKey();
	static unsigned char* generateSecretIV();
private:
	Crypto();
	~Crypto();
};
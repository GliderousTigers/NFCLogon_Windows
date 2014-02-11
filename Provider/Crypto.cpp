#include "Crypto.h"
#include <stdlib.h>   
#include <openssl\rand.h>

RSA* Crypto::GenerateKeyPair()
{
	BIGNUM* exp = BN_new();
	BN_set_word(exp, DEFAULT_RSA_EXP);

	RSA* rsaKeys = RSA_new();

	RSA_generate_key_ex(rsaKeys, DEFAULT_RSA_SIZE, exp, NULL);
	return rsaKeys;
}

RSA* Crypto::getPublicKey(BIGNUM modulus, BIGNUM exponent)
{
	RSA* key = RSA_new();
	key->n = &modulus;
	key->e = &exponent;

	return key;
}

unsigned char* Crypto::aes_encrypt(unsigned char* key, unsigned char* IV, unsigned char* data, int* len)
{
	EVP_CIPHER_CTX* e;
	EVP_CIPHER_CTX_init(e);
	EVP_EncryptInit_ex(e, EVP_aes_256_cbc(), NULL,key, IV);
	/* max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes */
	int c_len = *len + AES_BLOCK_SIZE, f_len = 0;
	unsigned char* ciphertext = (unsigned char*)malloc(c_len);

	/* allows reusing of 'e' for multiple encryption cycles */
	EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);

	/* update ciphertext, c_len is filled with the length of ciphertext generated,
	*len is the size of plaintext in bytes */
	EVP_EncryptUpdate(e, ciphertext, &c_len, data, *len);

	/* update ciphertext with the final remaining bytes */
	EVP_EncryptFinal_ex(e, ciphertext + c_len, &f_len);

	*len = c_len + f_len;
	return ciphertext;
}

unsigned char* Crypto::aes_decrypt(unsigned char* key, unsigned char* IV, unsigned char* ciphertext, int* len)
{
	EVP_CIPHER_CTX* d;
	EVP_CIPHER_CTX_init(d);
	EVP_DecryptInit_ex(d, EVP_aes_256_cbc(), NULL, key, IV);
	/* because we have padding ON, we must allocate an extra cipher block size of memory */
	int p_len = *len, f_len = 0;
	unsigned char* plaintext = (unsigned char*)malloc(p_len + AES_BLOCK_SIZE);

	EVP_DecryptInit_ex(d, NULL, NULL, NULL, NULL);
	EVP_DecryptUpdate(d, plaintext, &p_len, ciphertext, *len);
	EVP_DecryptFinal_ex(d, plaintext + p_len, &f_len);

	*len = p_len + f_len;
	return plaintext;
}

unsigned char* Crypto::generateSecretKey()
{
	unsigned char* key = new unsigned char[AES_BLOCK_SIZE / 8];

	RAND_bytes(key, sizeof(key));

	return key;
}
unsigned char* generateSecretIV()
{
	unsigned char* IV = new unsigned char[AES_BLOCK_SIZE / 8];
	RAND_bytes(IV, sizeof(IV));

	return IV;
}





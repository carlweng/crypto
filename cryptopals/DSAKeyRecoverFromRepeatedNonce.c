///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      The cryptopals crypto challenge/
//      Set 6/
//      Challenge 44/
//      DSA key recovery from repeated nonce
//      author: Chenkai Weng
//
//      
//                      Digital Signature Algorithm (DSA)
//
//  1. Parameter Generation
//      (1) Choose an N-bit prime p
//      (2) Choose an L-bit prime q such that p - 1 is a multiple of q
//      (3) Choose g, a number whose multiplicative order modulo p is q
//      (4) Parameters (p, q, g) are shared between users
//  2. Per-User Keys
//      (1) Choose a secret key x by some random method, where 0 < x < q
//      (2) Calculate the public key y = g^x mod p
//  3. Signing
//      (1) Choose an approved cryptographic hash function H
//      (2) Generate a random per-message value k where 1<k<q
//      (3) Calculate r = (g^k mod p) mod q. If r == 0, start again with a different k
//      (4) Calculate s = k^(-1) (H(m) + xr) mod q. If s == 0, start again with a different k
//      (5) The signature is (r, s)
//  4. Verifying
//      (1) Calculate w = s^(-1) mod q 
//      (2) Calculate u1 = H(m) * w mod q
//      (3) Calculate u2 = r * w mod q
//      (4) Calculate v = (g^(u1)*g^(u2) mod p) mod q
//      (5) The signature is valid if v == r
//
//
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
//#include <openssl/evp.h>

/*
void digest_message(const unsigned char *message, size_t message_len, unsigned char **digest, unsigned int *digest_len)
{
	EVP_MD_CTX *mdctx;

	if((mdctx = EVP_MD_CTX_create()) == NULL)
		handleErrors();

	if(1 != EVP_DigestInit_ex(mdctx, EVP_sha1(), NULL))
		handleErrors();

	if(1 != EVP_DigestUpdate(mdctx, message, message_len))
		handleErrors();

	if((*digest = (unsigned char *)OPENSSL_malloc(EVP_MD_size(EVP_sha1()))) == NULL)
		handleErrors();

	if(1 != EVP_DigestFinal_ex(mdctx, *digest, digest_len))
		handleErrors();

	EVP_MD_CTX_destroy(mdctx);
}*/

int main(void)
{
    mpz_t p, q, g;
    mpz_init_set_str(p, "0x800000000000000089e1855218a0e7dac38136ffafa72eda7859f2171e25e65eac698c1702578b07dc2a1076da241c76c62d374d8389ea5aeffd3226a0530cc565f3bf6b50929139ebeac04f48c3c84afb796d61e5a4f9a8fda812ab59494232c7d2b4deb50aa18ee9e132bfa85ac4374d7f9091abc3d015efc871a584471bb1", 0);
    mpz_init_set_str(q, "0xf4f47f05794b256174bba6e9b396a7707e563c5b", 0);
    mpz_init_set_str(g, "0x5958c9d3898b224b12672c0b98e06c60df923cb8bc999d119458fef538b8fa4046c8db53039db620c094c9fa077ef389b5322a559946a71903f990f1f7e0e025e2d7f7cf494aff1a0470f5b64c36b625a097f1651fe775323556fe00b3608c887892878480e99041be601a62166ca6894bdd41a7054ec89f756ba9fc95302291", 0);

    mpz_t y;
    mpz_init_set_str(y, "0x2d026f4bf30195ede3a088da85e398ef869611d0f68f0713d51c9c1a3a26c95105d915e2d8cdf26d056b86b8a7b85519b1c23cc3ecdc6062650462e3063bd179c2a6581519f674a61f1d89a1fff27171ebc1b93d4dc57bceb7ae2430f98a6a4d83d8279ee65d71c1203d2c96d65ebbf7cce9d32971c3de5084cce04a2e147821", 0);

    mpz_t msg[6];
    for(int i = 0; i < 6; i++)
        mpz_init(msg[i]);
    mpz_set_str(msg[0], "0xa4db3de27e2db3e5ef085ced2bced91b82e0df19", 0);
    mpz_set_str(msg[1], "0xa4db3de27e2db3e5ef085ced2bced91b82e0df19", 0);
    mpz_set_str(msg[2], "0x21194f72fe39a80c9c20689b8cf6ce9b0e7e52d4", 0);
    mpz_set_str(msg[3], "0xd22804c4899b522b23eda34d2137cd8cc22b9ce8", 0);
    mpz_set_str(msg[4], "0xbc7ec371d951977cba10381da08fe934dea80314", 0);
    mpz_set_str(msg[5], "0xd6340bfcda59b6b75b59ca634813d572de800e8f", 0);

    mpz_t r[3], s[6];
    mpz_init_set_str(r[0], "1105520928110492191417703162650245113664610474875", 0);
    mpz_init_set_str(r[1], "51241962016175933742870323080382366896234169532", 0);
    mpz_init_set_str(r[2], "228998983350752111397582948403934722619745721541", 0);
    mpz_init_set_str(s[0], "1267396447369736888040262262183731677867615804316", 0);
    mpz_init_set_str(s[1], "29097472083055673620219739525237952924429516683", 0);
    mpz_init_set_str(s[2], "277954141006005142760672187124679727147013405915", 0);
    mpz_init_set_str(s[3], "1021643638653719618255840562522049391608552714967", 0);
    mpz_init_set_str(s[4], "506591325247687166499867321330657300306462367256", 0);
    mpz_init_set_str(s[5], "458429062067186207052865988429747640462282138703", 0);

    mpz_t mMinus[3];
    mpz_t SMinus[3];
    mpz_t SMinusInvert[3];
    mpz_t k[3];
    mpz_t rTest;
    mpz_init(rTest);
    for(int i = 0; i < 3; i++)
    {
        mpz_init(mMinus[i]);
        mpz_sub(mMinus[i], msg[i + 3], msg[i]);

        mpz_init(SMinus[i]);
        mpz_sub(SMinus[i], s[i + 3], s[i]);

        mpz_init(SMinusInvert[i]);
        mpz_invert(SMinusInvert[i], SMinus[i], q);

        mpz_init(k[i]);
        mpz_mul(k[i], SMinusInvert[i], mMinus[i]);
        mpz_mod(k[i], k[i], q);

        mpz_powm(rTest, g, k[i], p);
        mpz_mod(rTest, rTest, q);
        gmp_printf("r:\n%Zd\n", rTest);
    }

    mpz_t temp, rInvert[6], x[6], yChance[6];
    mpz_init(temp);
    for(int i = 0; i < 6; i++)
    {
        int index = i>2?i-3:i;  
        mpz_mul(temp, s[i], k[index]);
        mpz_sub(temp, temp, msg[i]);
        
        mpz_init(rInvert[i]);
        mpz_invert(rInvert[i], r[index], q);
        mpz_mul(temp, temp, rInvert[i]);

        mpz_init(x[i]);
        mpz_mod(x[i], temp, q);

        mpz_init(yChance[i]);
        mpz_powm(yChance[i], g, x[i], p);
    }

    if(mpz_cmp(y, yChance[0]) == 0)
    {
        gmp_printf("Right private key:\n%ZX\n", x[0]);
    }

    return 0;
}
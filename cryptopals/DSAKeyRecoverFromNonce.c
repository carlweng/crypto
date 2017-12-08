///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      The cryptopals crypto challenge/
//      Set 6/
//      Challenge 43/
//      DSA key recovery from nonce
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
//  Little tip: compare y^r with possible g^(x*r), which will avoid the divide operation


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

int main(void)
{
    unsigned long int seed = 123456789;
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed);

    mpz_t p, q, g;
    mpz_init_set_str(p, "0x800000000000000089e1855218a0e7dac38136ffafa72eda7859f2171e25e65eac698c1702578b07dc2a1076da241c76c62d374d8389ea5aeffd3226a0530cc565f3bf6b50929139ebeac04f48c3c84afb796d61e5a4f9a8fda812ab59494232c7d2b4deb50aa18ee9e132bfa85ac4374d7f9091abc3d015efc871a584471bb1", 0);
    mpz_init_set_str(q, "0xf4f47f05794b256174bba6e9b396a7707e563c5b", 0);
    mpz_init_set_str(g, "0x5958c9d3898b224b12672c0b98e06c60df923cb8bc999d119458fef538b8fa4046c8db53039db620c094c9fa077ef389b5322a559946a71903f990f1f7e0e025e2d7f7cf494aff1a0470f5b64c36b625a097f1651fe775323556fe00b3608c887892878480e99041be601a62166ca6894bdd41a7054ec89f756ba9fc95302291", 0);

    mpz_t y, yExpr;
    mpz_init(yExpr);
    mpz_init_set_str(y, "0x84ad4719d044495496a3201c8ff484feb45b962e7302e56a392aee4abab3e4bdebf2955b4736012f21a08084056b19bcd7fee56048e004e44984e2f411788efdc837a0d2e5abb7b555039fd243ac01f0fb2ed1dec568280ce678e931868d23eb095fde9d3779191b8c0299d6e07bbb283e6633451e535c45513b2d33c99ea17", 0);
    mpz_t HashMsg;
    mpz_init_set_str(HashMsg, "0xd2d0714f014a9784047eaeccf956520045c45265", 0);

    mpz_t r, s;
    mpz_init_set_str(r, "548099063082341131477253921760299949438196259240", 0);
    mpz_init_set_str(s, "857042759984254168557880549501802188789837994940", 0);
    mpz_powm(yExpr, y, r, p);

    mpz_t xr;
    mpz_t yChance;
    mpz_t k;
    mpz_t temp;
    mpz_init(temp);
    mpz_init(k);
    mpz_init_set_ui(xr, 0);
    mpz_init_set_ui(yChance, 0);
    while(mpz_cmp(yExpr, yChance) != 0)
    {
        mpz_urandomb(k, state, 16);
        mpz_mul(temp, s, k);
        mpz_sub(temp, temp, HashMsg);
        mpz_mod(xr, temp, q);
        mpz_powm(yChance, g, xr, p);
    }

    mpz_t x, rInvert;
    mpz_init(x);
    mpz_init(rInvert);
    mpz_invert(rInvert, r, q);
    mpz_mul(x, xr, rInvert);
    mpz_mod(x, x, q);

    gmp_printf("k:\n%ZX\n", k);
    gmp_printf("x:\n%ZX\n", x);

    return 0;
}
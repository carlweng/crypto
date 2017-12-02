#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

unsigned char* readFrame()
{
    unsigned char *frame = (unsigned char*)malloc(sizeof(unsigned char) * 1024 * 3 + 1);
    FILE *fp;
    fp = fopen("frame/Frame0", "r");
    fread(frame, 1024 * 3, 1, fp);
    return frame;
}

void Hex2string(char* messageHex)
{
    char *mHex = (char*)malloc(sizeof(char)*20);
    int lenHex = strlen(messageHex);
    int j = 0;
    int i;
    for(i = lenHex-16; i < lenHex; i++)
        mHex[j++] = messageHex[i];
    char *message = (char*)malloc(sizeof(char)*10);
    char high, low;
    for(i = 0; i < 16; i+=2)
    {
        high = mHex[i];
        low = mHex[i+1];
        message[i/2] = high * 16 + low;
    }
    printf("message:\n%s\n", message);
}

int main(void)
{
    unsigned char *frame = readFrame();
    unsigned char *nstr = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    unsigned char *estr = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    unsigned char *cstr = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    cstr[0] = '0';
    cstr[1] = 'x';
    estr[0] = '0';
    estr[1] = 'x';
    nstr[0] = '0';
    nstr[1] = 'x';
    int i;
    for(i = 2; i < 258; i++)
    {
        nstr[i] = frame[i - 2];
        estr[i] = frame[i - 2 + 256];
        cstr[i] = frame[i - 2 + 512];
    }
    //printf("%s\n%s\n%s\n%s\n", frame, nstr, estr, cstr);
    
    mpz_t e;
    mpz_t n;
    mpz_t c;
    mpz_t d;
    mpz_init(c);
    mpz_init(e);
    mpz_init(n);
    mpz_init(d);
    mpz_init_set_str(c, cstr, 0);
    mpz_init_set_str(e, estr, 0);
    mpz_init_set_str(n, nstr, 0);
    //gmp_printf("c: %ZX\ne: %ZX\nn: %ZX\n", c, e, n);

    mpz_t p;
    mpz_t q;
    mpz_init(p);
    mpz_init(q);

    mpz_t permutation;
    mpz_t permutationFormer;
    mpz_init(permutation);
    mpz_init(permutationFormer);
    mpz_powm(permutation, c, e, n);
    while(mpz_cmp(permutation, c) != 0)
    {
        mpz_set(permutationFormer, permutation);
        mpz_powm(permutation, permutation, e, n);
    }

    mpz_t m;
    mpz_init(m);
    mpz_set(m, permutationFormer);
    //gmp_printf("m:\n%ZX\n", m);

    char *messageHex = (char*)malloc(sizeof(char)*1100);
    gmp_sprintf(messageHex, "%ZX", m);
    //printf("messageHex:\n%s\n", messageHex);
    Hex2string(messageHex);

    mpz_t cMinus;
    mpz_init(cMinus);
    mpz_sub(cMinus, permutation, c);
    mpz_gcd(p, cMinus, n);
    mpz_cdiv_q(q, n, p);
    gmp_printf("found p:\n%ZX\nq:\n%ZX\n", p, q);

    mpz_t pSub, qSub, totient;
    mpz_init(pSub);
    mpz_init(qSub);
    mpz_sub_ui(pSub, p, 1);
    mpz_sub_ui(qSub, q, 1);
    mpz_mul(totient, pSub, qSub);

    mpz_invert(d, e, totient);
    gmp_printf("d:\n%ZX\n", d);


    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

unsigned char* readFrame(char* filename)
{
    unsigned char *frame = (unsigned char*)malloc(sizeof(unsigned char) * 1024 * 3 + 1);
    FILE *fp;
    fp = fopen(filename, "r");
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
        if(high <= '9')
            high = high - 48;
        else
            high = high - 55;
        low = mHex[i+1];
        if(low <= '9')
            low = low - 48;
        else
            low = low - 55;
        message[i/2] = high * 16 + low;
    }
    message[8] = '\0';
    printf("message:\n%s\n", message);
}


int main(void)
{
    char *filename3 = "frame/Frame3";
    unsigned char *frame3 = readFrame(filename3);
    unsigned char *nstr3 = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    unsigned char *estr3 = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    unsigned char *cstr3 = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    cstr3[0] = '0';
    cstr3[1] = 'x';
    estr3[0] = '0';
    estr3[1] = 'x';
    nstr3[0] = '0';
    nstr3[1] = 'x';
    int i;
    for(i = 2; i < 258; i++)
    {
        nstr3[i] = frame3[i - 2];
        estr3[i] = frame3[i - 2 + 256];
        cstr3[i] = frame3[i - 2 + 512];
    }
    //printf("%s\n%s\n%s\n", nstr3, estr3, cstr3);

    char *filename8 = "frame/Frame8";
    unsigned char *frame8 = readFrame(filename8);
    unsigned char *nstr8 = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    unsigned char *estr8 = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    unsigned char *cstr8 = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    cstr8[0] = '0';
    cstr8[1] = 'x';
    estr8[0] = '0';
    estr8[1] = 'x';
    nstr8[0] = '0';
    nstr8[1] = 'x';
    for(i = 2; i < 258; i++)
    {
        nstr8[i] = frame8[i - 2];
        estr8[i] = frame8[i - 2 + 256];
        cstr8[i] = frame8[i - 2 + 512];
    }
    //printf("\n%s\n%s\n%s\n", nstr8, estr8, cstr8);

    char *filename12 = "frame/Frame12";
    unsigned char *frame12 = readFrame(filename12);
    unsigned char *nstr12 = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    unsigned char *estr12 = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    unsigned char *cstr12 = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    cstr12[0] = '0';
    cstr12[1] = 'x';
    estr12[0] = '0';
    estr12[1] = 'x';
    nstr12[0] = '0';
    nstr12[1] = 'x';
    for(i = 2; i < 258; i++)
    {
        nstr12[i] = frame12[i - 2];
        estr12[i] = frame12[i - 2 + 256];
        cstr12[i] = frame12[i - 2 + 512];
    }
    //printf("\n%s\n%s\n%s\n", nstr12, estr12, cstr12);
    
    mpz_t e;
    mpz_init_set_str(e, estr3, 0);

    mpz_t n1;
    mpz_t n2;
    mpz_t n3;
    mpz_init_set_str(n1, nstr3, 0);
    mpz_init_set_str(n2, nstr8, 0);
    mpz_init_set_str(n3, nstr12, 0);

    mpz_t c1;
    mpz_t c2;
    mpz_t c3;
    mpz_init_set_str(c1, cstr3, 0);
    mpz_init_set_str(c2, cstr8, 0);
    mpz_init_set_str(c3, cstr12, 0);

    mpz_t n1Muln2;
    mpz_t n1Muln3;
    mpz_t n2Muln3;
    mpz_t n1Muln2Muln3;
    mpz_init(n1Muln2);
    mpz_init(n1Muln3);
    mpz_init(n2Muln3);
    mpz_init(n1Muln2Muln3);
    mpz_mul(n1Muln2, n1, n2);
    mpz_mul(n1Muln3, n1, n3);
    mpz_mul(n2Muln3, n2, n3);
    mpz_mul(n1Muln2Muln3, n1Muln2, n3);

    mpz_t n1Inverse;
    mpz_t n2Inverse;
    mpz_t n3Inverse;
    mpz_init(n1Inverse);
    mpz_init(n2Inverse);
    mpz_init(n3Inverse);
    mpz_invert(n1Inverse, n2Muln3, n1);
    mpz_invert(n2Inverse, n1Muln3, n2);
    mpz_invert(n3Inverse, n1Muln2, n3);

    mpz_t n1Muln1In;
    mpz_t n2Muln2In;
    mpz_t n3Muln3In;
    mpz_init(n1Muln1In);
    mpz_init(n2Muln2In);
    mpz_init(n3Muln3In);
    mpz_mul(n1Muln1In, n2Muln3, n1Inverse);
    mpz_mul(n2Muln2In, n1Muln3, n2Inverse);
    mpz_mul(n3Muln3In, n1Muln2, n3Inverse);

    mpz_t c1n1n1In;
    mpz_t c2n2n2In;
    mpz_t c3n3n3In;
    mpz_init(c1n1n1In);
    mpz_init(c2n2n2In);
    mpz_init(c3n3n3In);
    mpz_mul(c1n1n1In, c1, n1Muln1In);
    mpz_mul(c2n2n2In, c2, n2Muln2In);
    mpz_mul(c3n3n3In, c3, n3Muln3In);

    mpz_t c1addc2;
    mpz_t c1addc2addc3;
    mpz_init(c1addc2);
    mpz_init(c1addc2addc3);
    mpz_add(c1addc2, c1n1n1In, c2n2n2In);
    mpz_add(c1addc2addc3, c1addc2, c3n3n3In);

    mpz_t x;
    mpz_init(x);
    mpz_powm_ui(x, c1addc2addc3, 1, n1Muln2Muln3);

    mpz_t m;
    mpz_t cRe;
    mpz_init(m);
    mpz_init(cRe);

    mpz_root(m, x, 5);
    mpz_powm(cRe, m, e, n1);
    if(mpz_cmp(c1, cRe) == 0)
        printf("Right m\n");

    char *messageHex = (char*)malloc(sizeof(char)*1100);
    gmp_sprintf(messageHex, "%ZX", m);
    Hex2string(messageHex);

    printf("messageHex:\n%s\n", messageHex);

    return 0;
}

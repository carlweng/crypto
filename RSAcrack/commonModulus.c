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
    char *filename0 = "frame/Frame0";
    unsigned char *frame0 = readFrame(filename0);
    unsigned char *nstr0 = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    unsigned char *estr0 = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    unsigned char *cstr0 = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    cstr0[0] = '0';
    cstr0[1] = 'x';
    estr0[0] = '0';
    estr0[1] = 'x';
    nstr0[0] = '0';
    nstr0[1] = 'x';
    int i;
    for(i = 2; i < 258; i++)
    {
        nstr0[i] = frame0[i - 2];
        estr0[i] = frame0[i - 2 + 256];
        cstr0[i] = frame0[i - 2 + 512];
    }
    //printf("%s\n%s\n%s\n", nstr3, estr3, cstr3);

    char *filename4 = "frame/Frame4";
    unsigned char *frame4 = readFrame(filename4);
    unsigned char *nstr4 = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    unsigned char *estr4 = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    unsigned char *cstr4 = (unsigned char*)malloc(sizeof(unsigned char) * 1027);
    cstr4[0] = '0';
    cstr4[1] = 'x';
    estr4[0] = '0';
    estr4[1] = 'x';
    nstr4[0] = '0';
    nstr4[1] = 'x';
    for(i = 2; i < 258; i++)
    {
        nstr4[i] = frame4[i - 2];
        estr4[i] = frame4[i - 2 + 256];
        cstr4[i] = frame4[i - 2 + 512];
    }
    //printf("\n%s\n%s\n%s\n", nstr8, estr8, cstr8);
    
    mpz_t n;
    mpz_init_set_str(n, nstr0, 0);

    mpz_t e1;
    mpz_t e2;
    mpz_init_set_str(e1, estr0, 0);
    mpz_init_set_str(e2, estr4, 0);

    mpz_t c1;
    mpz_t c2;
    mpz_init_set_str(c1, cstr0, 0);
    mpz_init_set_str(c2, cstr4, 0);

    mpz_t g, s, t;
    mpz_init(g);
    mpz_init(s);
    mpz_init(t);
    mpz_gcdext(g, s, t, e1, e2);

    mpz_t c1s, c2t, c1sc2t;
    mpz_init(c1s);
    mpz_init(c2t);
    mpz_init(c1sc2t);
    mpz_powm(c1s, c1, s, n);
    mpz_powm(c2t, c2, t, n);
    mpz_mul(c1sc2t, c2t, c1s);

    mpz_t m;
    mpz_init(m);
    mpz_powm_ui(m, c1sc2t, 1, n);


    mpz_t cRe;
    mpz_init(cRe);

    mpz_powm(cRe, m, e1, n);
    if(mpz_cmp(c1, cRe) == 0)
        printf("Right m\n");

    char *messageHex = (char*)malloc(sizeof(char)*1100);
    gmp_sprintf(messageHex, "%ZX", m);
    Hex2string(messageHex);

    printf("messageHex:\n%s\n", messageHex);

    return 0;
}

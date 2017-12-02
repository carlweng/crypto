#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

unsigned char* readFrame()
{
    unsigned char *frame = (unsigned char*)malloc(sizeof(unsigned char) * 1024 * 3 + 1);
    FILE *fp;
    fp = fopen("frame/Frame10", "r");
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
    gmp_printf("c: %ZX\ne: %ZX\nn: %Zx\n", c, e, n);


    mpz_t k, y;
    mpz_init(k);
    mpz_init(y);
    mpz_init(d);

    mpz_t nRoot2, kExp2;
    mpz_init(nRoot2);
    mpz_init(kExp2);
    mpz_sqrt(nRoot2, n);
    mpz_add_ui(k, nRoot2, 1);
    mpz_pow_ui(kExp2, k, 2);
    mpz_sub(y, kExp2, n);
    mpz_set_ui(d, 1);

    mpz_t ySqrt, ySqrtReminder, nDiv2;
    mpz_init(ySqrt);
    mpz_init(ySqrtReminder);
    mpz_init(nDiv2);
    mpz_sqrtrem(ySqrt, ySqrtReminder, y);
    mpz_fdiv_q_ui(nDiv2, n, 2);

    while((mpz_cmp_ui(ySqrtReminder, 0) != 0) && (mpz_cmp(ySqrt, nDiv2) >= 0))
    {
        mpz_t kMul2, temp1;
        mpz_init(kMul2);
        mpz_init(temp1);
        mpz_mul_ui(kMul2, k, 2);
        mpz_add(temp1, kMul2, d);
        mpz_add(y, temp1, y);

        mpz_add_ui(d, d, 2);

        mpz_sqrtrem(ySqrt, ySqrtReminder, y);
    }

    mpz_t p, q;
    mpz_init(p);
    mpz_init(q);
    mpz_add(q, ySqrt, k);
    mpz_sub(p, k, ySqrt);
    gmp_printf("p:\n%ZX\nq:\n%ZX\n", p, q);
/*    gmp_printf("y:\n%ZX\n", y);
    mpz_t xr, yr, Nplusy, xMinusy, xPlusy;
    mpz_inits(xr, yr, Nplusy, xMinusy, xPlusy);
    mpz_add(Nplusy, n, y);
    mpz_sqrt(xr, Nplusy);
    mpz_sqrt(yr, y);
    mpz_sub(xMinusy, xr, yr);
    mpz_set(p, xMinusy);
    mpz_add(xPlusy, xr, yr);
    mpz_sqrt(q, xPlusy);*/

    mpz_t pSub, qSub, totient;
    mpz_init(pSub);
    mpz_init(qSub);
    mpz_init(totient);
    mpz_sub_ui(pSub, p, 1);
    mpz_sub_ui(qSub, q, 1);
    mpz_mul(totient, pSub, qSub);
    gmp_printf("totient:\n%ZX\n", totient);

    mpz_t dKey;
    mpz_init(dKey);
    mpz_invert(dKey, e, totient);
    gmp_printf("e:\n%ZX\n", e);
    gmp_printf("d:\n%ZX\n", dKey);

    mpz_t m;
    mpz_init(m);
    mpz_powm(m, c, dKey, n);
    gmp_printf("m:\n%ZX\n", m);

    char *messageHex = (char*)malloc(sizeof(char)*1100);
    gmp_sprintf(messageHex, "%ZX", m);
    printf("messageHex:\n%s\n", messageHex);
    Hex2string(messageHex);
    return 0;
}

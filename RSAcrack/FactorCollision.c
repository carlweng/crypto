#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

unsigned char* readFrame(char* filename)
{
    unsigned char *frame = (unsigned char*)malloc(sizeof(unsigned char) * 1024 * 20 + 1);
    FILE *fp;
    fp = fopen(filename, "r");
    fread(frame, 1024 * 20, 1, fp);
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
    mpz_t N[20];
    char *filename = "NinFrames.txt";
    unsigned char *frame = readFrame(filename);
    int i, j;
    for(i = 0; i < 20; i++)
    {
        unsigned char *nStr = (unsigned char*)malloc(sizeof(unsigned char) * 258);
        nStr[0] = '0';
        nStr[1] = 'x';
        for(j = 2; j < 258; j++)
        {
            nStr[j] = frame[i * 256 + j - 2];
        }
        nStr[258] = '\0';
        mpz_init_set_str(N[i], nStr, 0);
        //gmp_printf("n if\n%ZX\n", N[i]);
    }

    int n1, n2, flag = 0;
    mpz_t gcd, p;
    mpz_init(gcd);
    mpz_init(p);
    for(i = 0; i < 19; i++)
    {
        for(j = i + 1; j < 20; j++)
        {
            mpz_gcd(gcd, N[i], N[j]);
            if(mpz_cmp_ui(gcd, 1) > 0)
            {
                n1 = i;
                n2 = j;
                printf("n in frame %d and frame %d have factor\n", i, j);
                gmp_printf("the factor is\n%ZX\n", gcd);
                mpz_set(p, gcd);
                flag = 1;
                break;
            }
        }
        if(flag)
            break;
    }

    mpz_t q1, q2;
    mpz_init(q1);
    mpz_init(q2);
    mpz_cdiv_q(q1, N[n1], p);
    mpz_cdiv_q(q2, N[n2], p);

    mpz_t temp1, temp2, temp3;
    mpz_t co1, co2;
    mpz_init(temp1);
    mpz_init(temp2);
    mpz_init(temp3);
    mpz_init(co1);
    mpz_init(co2);
    mpz_sub_ui(temp1, p, 1);
    mpz_sub_ui(temp2, q1, 1);
    mpz_sub_ui(temp3, q2, 1);
    mpz_mul(co1, temp1, temp2);
    mpz_powm_ui(co1, co1, 1, N[n1]);
    mpz_mul(co2, temp1, temp3);
    mpz_powm_ui(co2, co2, 1, N[n2]);

    gmp_printf("cotient of frame %d is:\n%ZX\n", n1, co1);
    gmp_printf("cotient of frame %d is:\n%ZX\n", n2 + 1, co2);

    mpz_t e1, e2, c1, c2;
    mpz_init_set_str(e1, "0x10001", 0);
    mpz_init_set_str(c1, "0x0251025DC5FB84476581D0F67C640D8927DA6D083627C9C29F3174C17CFE316A6218194DD4BE03D30EF9ECCBB4C609673D853590DD122B151DCFD6D75FD202DC2C758E897BABE0A4CD842FF35D086CF4E34EFBD09E8FF9FBFB4B5254CA2323A463139ABD16E301C37F683579BA624EFBB297B9E6D5A1C68F75EB4BADF9AA198C", 0);
    mpz_init_set_str(e2, "0x10001", 0);
    mpz_init_set_str(c2, "0x45D8BD62BBF9966C81722D6D4AD5E6E91FD5258C8B0747CA166237D167D5C881B100D83D73352F18A60914963CA8F7DF9B9211273C8D7EDAC87132AADAC33DEF0BDA6C9EA91750818D869990521C6BA0A10BC1AC2273282FA4AC47EFBEEE99B2D35EBDA2019D1EF8BF24B5017FA8481B372362AAE138043A00D8761BCDCA80BC", 0);

    mpz_t d1, d2;
    mpz_init(d1);
    mpz_init(d2);
    mpz_invert(d1, e1, co1);
    mpz_invert(d2, e2, co2);

    mpz_t m1, m2;
    mpz_init(m1);
    mpz_init(m2);
    mpz_powm(m1, c1, d1, N[n1]);
    mpz_powm(m2, c2, d2, N[n2]);

    char *messageHex1 = (char*)malloc(sizeof(char)*1100);
    gmp_sprintf(messageHex1, "%ZX", m1);
    Hex2string(messageHex1);

    char *messageHex2 = (char*)malloc(sizeof(char)*1100);
    gmp_sprintf(messageHex2, "%ZX", m2);
    Hex2string(messageHex2);


/*    mpz_t cRe;
    mpz_init(cRe);

    mpz_powm(cRe, m, e1, n);
    if(mpz_cmp(c1, cRe) == 0)
        printf("Right m\n");

    char *messageHex = (char*)malloc(sizeof(char)*1100);
    gmp_sprintf(messageHex, "%ZX", m);
    Hex2string(messageHex);

    printf("messageHex:\n%s\n", messageHex);*/

    return 0;
}

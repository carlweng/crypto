#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define LEFTROTATE(A,N) ((A)<<(N))|((A)>>(32-(N)))

uint32_t MK[4];
uint32_t rk[32];
//uint32_t rk[32] = {0xf12186f9, 0x41662b61, 0x5a6ab19a, 0x7ba92077, 0x367360f4, 0x776a0c61, 0xb6bb89b3, 0x24763151, 0xa520307c, 0xb7584dbd, 0xc30753ed, 0x7ee55b57, 0x6988608c, 0x30d895b7, 0x44ba14af, 0x104495a1, 0xd120b428, 0x73b55fa3, 0xcc874966, 0x92244439, 0xe89e641f, 0x98ca015a, 0xc7159060, 0x99e1fd2e, 0xb79bd80c, 0x1d2115b0, 0x0e228aeb, 0xf1780c81, 0x428d3654, 0x62293496, 0x01cf72e5, 0x9124a012};
uint32_t FK[4] = {0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc};
uint32_t K[36];
uint32_t CK[32] = {
    0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269, 
    0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
    0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
    0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
    0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
    0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
    0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
    0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279};

int Sbox[16][16] = {
    {0xd6,0x90,0xe9,0xfe,0xcc,0xe1,0x3d,0xb7,0x16,0xb6,0x14,0xc2,0x28,0xfb,0x2c,0x05},
    {0x2b,0x67,0x9a,0x76,0x2a,0xbe,0x04,0xc3,0xaa,0x44,0x13,0x26,0x49,0x86,0x06,0x99},
    {0x9c,0x42,0x50,0xf4,0x91,0xef,0x98,0x7a,0x33,0x54,0x0b,0x43,0xed,0xcf,0xac,0x62},
    {0xe4,0xb3,0x1c,0xa9,0xc9,0x08,0xe8,0x95,0x80,0xdf,0x94,0xfa,0x75,0x8f,0x3f,0xa6},
    {0x47,0x07,0xa7,0xfc,0xf3,0x73,0x17,0xba,0x83,0x59,0x3c,0x19,0xe6,0x85,0x4f,0xa8},
    {0x68,0x6b,0x81,0xb2,0x71,0x64,0xda,0x8b,0xf8,0xeb,0x0f,0x4b,0x70,0x56,0x9d,0x35},    
    {0x1e,0x24,0x0e,0x5e,0x63,0x58,0xd1,0xa2,0x25,0x22,0x7c,0x3b,0x01,0x21,0x78,0x87},    
    {0xd4,0x00,0x46,0x57,0x9f,0xd3,0x27,0x52,0x4c,0x36,0x02,0xe7,0xa0,0xc4,0xc8,0x9e},    
    {0xea,0xbf,0x8a,0xd2,0x40,0xc7,0x38,0xb5,0xa3,0xf7,0xf2,0xce,0xf9,0x61,0x15,0xa1},    
    {0xe0,0xae,0x5d,0xa4,0x9b,0x34,0x1a,0x55,0xad,0x93,0x32,0x30,0xf5,0x8c,0xb1,0xe3},    
    {0x1d,0xf6,0xe2,0x2e,0x82,0x66,0xca,0x60,0xc0,0x29,0x23,0xab,0x0d,0x53,0x4e,0x6f},    
    {0xd5,0xdb,0x37,0x45,0xde,0xfd,0x8e,0x2f,0x03,0xff,0x6a,0x72,0x6d,0x6c,0x5b,0x51},    
    {0x8d,0x1b,0xaf,0x92,0xbb,0xdd,0xbc,0x7f,0x11,0xd9,0x5c,0x41,0x1f,0x10,0x5a,0xd8},    
    {0x0a,0xc1,0x31,0x88,0xa5,0xcd,0x7b,0xbd,0x2d,0x74,0xd0,0x12,0xb8,0xe5,0xb4,0xb0},    
    {0x89,0x69,0x97,0x4a,0x0c,0x96,0x77,0x7e,0x65,0xb9,0xf1,0x09,0xc5,0x6e,0xc6,0x84},    
    {0x18,0xf0,0x7d,0xec,0x3a,0xdc,0x4d,0x20,0x79,0xee,0x5f,0x3e,0xd7,0xcb,0x39,0x48}    
    };

int clength32;

void binaryPrint(uint32_t reg)
{
	uint32_t num = reg;
	int binaryStack[33] = {0};
	int index = 31;
	for(int i = 0; i < 32; i++)
	{	
		binaryStack[index] = num % 2;
		num = num / 2;
		index--;
	}
	for(int i = 0; i < 32; i++)
	{
		printf("%d  ", binaryStack[i]);
	}
	printf("  %lld", reg);
	printf("\n");
}

unsigned char* uint322uchar(uint32_t struint32[], int length32)
{
    int lengthuchar = length32 * 4;
    unsigned char *struchar = (unsigned char*)malloc(sizeof(unsigned char)*lengthuchar+1);
    for(int i = 0; i < length32; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            struchar[i*4+j] = (unsigned char)((struint32[i] & (((uint32_t)0xff)<<(8*(3-j)))) >> (8*(3-j)));
        }
    }
    struchar[lengthuchar] = '\0';
    return struchar;
}

unsigned char* uchar2hex(unsigned char struchar[])
{
    int lengthuchar = strlen(struchar)%4==0?strlen(struchar):(strlen(struchar)/4+1)*4;//in case that it end with 00
    int lengthhex = lengthuchar*2 + 1;
    unsigned char ch[2];
    unsigned char *strhex = (unsigned char*)malloc(sizeof(unsigned char)*lengthhex+1);
    for(int i = 0; i < lengthuchar; i++)
    {
        ch[0] = struchar[i] / 16;
        ch[1] = struchar[i] % 16;
        for(int j = 0; j < 2; j++)
        {
            if(ch[j] < 10)
                strhex[i*2+j] = ch[j] + 48;
            else strhex[i*2+j] = ch[j] + 87;
        }
    }
    strhex[lengthhex] = '\0';
    //printf("strhex: %s\nlength: %d\n", strhex, lengthhex);
    return strhex;
}

uint32_t* hex2uint32(unsigned char hex[])
{
    int length = strlen(hex);
    unsigned char *str = (unsigned char*)malloc(sizeof(unsigned char)*length+1);
    unsigned char letter;
    unsigned char high, low;
    for(int i = 0; i < length; i+=2)
    {
        letter = 0;
        high = hex[i];
        low = hex[i+1];
        if(high <= '9')
            letter = (high - 48) * 16;
        else if(high <= 'F')
            letter = (high - 55) * 16;
        else letter = (high - 87) * 16;
        if(low <= '9')
            letter += (low - 48);
        else if(low <= 'F')
            letter += (low - 55);
        else letter += (low - 87);
        str[i/2] = letter;
    }
    int length32 = strlen(str)/4+1;
    uint32_t *str32 = (uint32_t*)malloc(sizeof(uint32_t)*length32);
    for(int i = 0; i < (length32-1)*4; i+=4)
    {
        str32[i/4] = 0;
        for(int j = 0; j < 4; j++)
        {
            str32[i/4] = str32[i/4] | ((uint32_t)(str[i+j]<<(8*(3-j))));
        }
    }
    return str32;
}

uint32_t Tau(uint32_t key)
{
    unsigned char a[4];
    unsigned char b[4];
    int high, low;
    uint32_t B = 0;
    //binaryPrint(key);
    for(int i = 0; i < 4; i++)
        a[i] = (unsigned char)((key&(((uint32_t)0xff)<<(8*(3-i)))) >> (8*(3-i)));
    for(int i = 0; i < 4; i++)
    {
        high = a[i] / 16;
        low = a[i] % 16;
        b[i] = Sbox[high][low];
        B |= ((uint32_t)b[i]) << (8*(3-i)); 
    }
    //binaryPrint(B);
    //B = B ^ LEFTROTATE(B,2) ^ LEFTROTATE(B,10) ^ LEFTROTATE(B,18) ^ LEFTROTATE(B,24);
    return B;
}

void KeyExtent()
{
    uint32_t *keyInput = hex2uint32("0123456789abcdeffedcba9876543210");
    uint32_t t, t1 , t2;
    for(int i = 0; i < 4; i++)
    {
        MK[i] = keyInput[i];
        K[i] = MK[i] ^ FK[i];
    }
    for(int i = 0; i < 32; i++)
    {
        t = Tau(K[i+1] ^ K[i+2] ^ K[i+3] ^ CK[i]);
        t1 = LEFTROTATE(t, 13);
        t2 = LEFTROTATE(t, 23);
        t = t ^ t1 ^ t2;
        rk[i] = t ^ K[i];
        K[i+4] = rk[i];
    }
    //printf("%s\n", uchar2hex(uint322uchar(rk, 32)));
}

uint32_t Ffunc(uint32_t x0, uint32_t x1, uint32_t x2, uint32_t x3, uint32_t rk)
{
    uint32_t t = Tau(x1 ^ x2 ^ x3 ^ rk);
    uint32_t t0 = LEFTROTATE(t,2);
    uint32_t t1 = LEFTROTATE(t,10); 
    uint32_t t2 = LEFTROTATE(t,18);
    uint32_t t3 = LEFTROTATE(t,24);
    t = t ^ t0 ^ t1 ^ t2 ^ t3;
    //t = t ^ LEFTROTATE(t,2) ^ LEFTROTATE(t,10) ^ LEFTROTATE(t,18) ^ LEFTROTATE(t,24);
    return (x0 ^ t);
}


uint32_t* Encrypt_ECB(uint32_t *msg, int length32)
{
    uint32_t X[36];
    uint32_t *Y = (uint32_t*)malloc(sizeof(uint32_t)*length32);
    int j;
    for(int i = 0; i < length32/4; i++)
    {
        for(j = 0; j < 4; j++)
            X[j] = msg[i*4+j];
        for(j = 0; j < 32; j++)
        {
            X[j+4] = Ffunc(X[j], X[j+1], X[j+2], X[j+3], rk[j]);
        }
        for(j = 0; j < 4; j++)
            Y[i*4+3-j] = X[j+32]; 
    }
    //display the rk and cipher for every round
/*    unsigned char *rkDisplay;
    unsigned char *xDisplay;
    for(int i = 0; i <32; i++)
    {
        rkDisplay = uchar2hex(uint322uchar(&rk[i], 1));
        xDisplay = uchar2hex(uint322uchar(&X[i+4], 1));
        printf("rk[%d]: %s    x[%d]: %s\n", i, rkDisplay, i, xDisplay);
    }*/

    return Y;
}

uint32_t* MessageEncrypt()
{
    //deal with the input hex message, padding it, and turn it to uint32_t
    unsigned char textInput[] = "0123456789abcdeffedcba9876543210";
    printf("plaintext: %s\n", textInput);
    int ucharLength = strlen(textInput) / 2;
    unsigned char *plaintextHex, *ciphertextHex;
    uint32_t *msguint32, *ctxuint32;
    int length32, length128;
    if(ucharLength % 16 == 0)///////////////////////////////%16
    {
        msguint32 = hex2uint32(textInput);
        length32 = ucharLength/4;
    }else{
        length128 = (ucharLength / 16) + 1;
        length32 = length128 * 4;
        plaintextHex = (unsigned char*)malloc(sizeof(unsigned char)*length32);
        int i;
        for(i = 0; i < ucharLength*2; i++)
            plaintextHex[i] = textInput[i];
        plaintextHex[i++] = 0x38;
        plaintextHex[i++] = 0x30;
        for(; i < length32 * 8; i++)
            plaintextHex[i] = 0x30;
        plaintextHex[length32*8] = '\0';
        msguint32 = hex2uint32(plaintextHex);
    }

    //encrypt message
    ctxuint32 = Encrypt_ECB(msguint32, length32);
    ciphertextHex = uchar2hex(uint322uchar(ctxuint32, length32));
    printf("ciphertext: %s\n", ciphertextHex);
    clength32 = length32;
    return ctxuint32;
}

void MessageDecrypt(uint32_t *ctx)
{
    uint32_t X[36];
    uint32_t *Y = (uint32_t*)malloc(sizeof(uint32_t)*clength32);
    int j;
    for(int i = 0; i < clength32/4; i++)
    {
        for(j = 0; j < 4; j++)
            X[j] = ctx[i*4+j];
        for(j = 0; j < 32; j++)
        {
            X[j+4] = Ffunc(X[j], X[j+1], X[j+2], X[j+3], rk[31-j]);
        }
        for(j = 0; j < 4; j++)
            Y[i*4+3-j] = X[j+32]; 
    }

    //display every round for decryption
    unsigned char *rkDisplay;
    unsigned char *xDisplay;
    for(int i = 0; i <32; i++)
    {
        rkDisplay = uchar2hex(uint322uchar(&rk[31-i], 1));
        xDisplay = uchar2hex(uint322uchar(&X[i+4], 1));
        printf("rk[%d]: %s    x[%d]: %s\n", 31-i, rkDisplay, i, xDisplay);
    }
    unsigned char *msghex = uchar2hex(uint322uchar(Y, clength32));
    printf("message decrypted: %s\n", msghex);
}

int main()
{
    uint32_t *ctxuint32;
    KeyExtent();
    ctxuint32 = MessageEncrypt();
    MessageDecrypt(ctxuint32);
    return 0;
}
#include <stdio.h>
#define DDgr 7
#define DGdr 32

unsigned long CalculateCRC(unsigned long dividend, unsigned long divisor, unsigned long dataDgr, unsigned long genDgr);
int GetMaxDgr(unsigned long polynomial);

int main()
{
     unsigned long dataWithCRC = 0;
    unsigned long generator = 0;
    unsigned long CRC =0;
    int genDgr, dataDgr;

    fscanf(stdin, "%lu %lu", &dataWithCRC, &generator);
    printf("dataWithCRC : %lu\n", dataWithCRC);
    printf("generator : %lu\n", generator);

    genDgr = GetMaxDgr(generator);
    dataDgr = GetMaxDgr(dataWithCRC);

    //get CRC code
    CRC = CalculateCRC(dataWithCRC, generator, dataDgr, genDgr);
    printf("CRC code = %lu\n", CRC);
    if(CRC ==0)
    {
        printf("correct message\n");
        dataWithCRC = dataWithCRC >> genDgr;
        printf("transmitted message : '%s'\n", (char*)&dataWithCRC);
        printf("original input : %x\n", dataWithCRC);
    }
    else
        printf("error\n");
}


unsigned long CalculateCRC(unsigned long dividend, unsigned long divisor, unsigned long dataDgr, unsigned long genDgr)
{
    int i;
    for(i =0; i <= dataDgr; i++)
    {
        if(dividend & (0x1 << (dataDgr -i)))//1로 설정된 dividend의 비트 포지션 스캔
        {
            dividend = dividend ^ (divisor << ((dataDgr - genDgr) -i));
            if(dividend < divisor)break;
        }
    }
    return dividend;
}

int GetMaxDgr(unsigned long polynomial)
{
    int i;
    for(i = DGdr; i >= 0; i--)
    {
        if((polynomial >> i) & 0x01) break;
    }
    return i;
}
/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 ϵ�� ����1��ַ�Զ�ʶ���������---------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966-------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.GXWMCU.com --------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����        */
/* ���Ҫ��������Ӧ�ô˴���,����������ע��ʹ����STC�����ϼ�����        */
/*---------------------------------------------------------------------*/

//��ʾ����Keil������������ѡ��Intel��8058оƬ�ͺŽ��б���
//�����ر�˵��,����Ƶ��һ��Ϊ11.0592MHz


#include "reg51.h"
#include "intrins.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;

//-----------------------------------------------

#define     SLAVE   1               //����ӻ����,0Ϊ�ӻ�1, 1Ϊ�ӻ�2

#if SLAVE == 0
#define     SAMASK  0x33            //�ӻ�1��ַ����λ
#define     SERADR  0x55            //�ӻ�1�ĵ�ַΪxx01,xx01
#define     ACKTST  0x78            //�ӻ�1Ӧ���������
#else
#define     SAMASK  0x3C            //�ӻ�2��ַ����λ
#define     SERADR  0x5A            //�ӻ�2�ĵ�ַΪxx01,10xx
#define     ACKTST  0x49            //�ӻ�2Ӧ���������
#endif

#define     URMD    2               //0:ʹ�ö�ʱ��2��Ϊ�����ʷ�����
                                    //1:ʹ�ö�ʱ��1��ģʽ0(16λ�Զ�����ģʽ)��Ϊ�����ʷ�����
                                    //2:ʹ�ö�ʱ��1��ģʽ2(8λ�Զ�����ģʽ)��Ϊ�����ʷ�����

sfr T2H   =   0xd6;                 //��ʱ��2��8λ
sfr T2L   =   0xd7;                 //��ʱ��2��8λ

sfr P0M1 = 0x93;
sfr P0M0 = 0x94;
sfr P1M1 = 0x91;
sfr P1M0 = 0x92;
sfr P2M1 = 0x95;
sfr P2M0 = 0x96;
sfr P3M1 = 0xb1;
sfr P3M0 = 0xb2;
sfr P4M1 = 0xb3;
sfr P4M0 = 0xb4;
sfr P5M1 = 0xC9;
sfr P5M0 = 0xCA;
sfr P6M1 = 0xCB;
sfr P6M0 = 0xCC;
sfr P7M1 = 0xE1;
sfr P7M0 = 0xE2;

sfr AUXR  =   0x8e;                 //�����Ĵ���                              

sfr SADDR =   0xA9;                 //�ӻ���ַ�Ĵ���
sfr SADEN =   0xB9;                 //�ӻ���ַ���μĴ���

void InitUart();

char count;

void main()
{
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
    P6M0 = 0x00;
    P6M1 = 0x00;
    P7M0 = 0x00;
    P7M1 = 0x00;

    InitUart();                     //��ʼ������
    ES = 1;
    EA = 1;
    while (1);
}

/*----------------------------
UART �жϷ������
-----------------------------*/
void Uart() interrupt 4 using 1
{
    if (TI)
    {
        TI = 0;                     //���TIλ
        if (count != 0)
        {
            count--;
            SBUF = ACKTST;          //��������Ӧ������
        }
        else
        {
            SM2 = 1;                //���������,�����¿�ʼ��ַ���
        }
    }
    if (RI)
    {
        RI = 0;                     //���RIλ
        SM2 = 0;                    //������ѡ�к�,�������ݽ���״̬
        count = 7;
        SBUF = ACKTST;              //��������Ӧ������
    }
}

/*----------------------------
��ʼ������
----------------------------*/
void InitUart()
{
    SADDR = SERADR;
    SADEN = SAMASK;
    SCON = 0xf8;                //���ô���Ϊ9λ�ɱ䲨����,ʹ�ܶ��ͨѶ���,(��TB8����Ϊ1,������PCֱ��ͨѶ����)
#if URMD == 0
    T2L = 0xd8;                 //���ò�������װֵ
    T2H = 0xff;                 //115200 bps(65536-18432000/4/115200)
    AUXR = 0x14;                //T2Ϊ1Tģʽ, ��������ʱ��2
    AUXR |= 0x01;               //ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
#elif URMD == 1
    AUXR = 0x40;                //��ʱ��1Ϊ1Tģʽ
    TMOD = 0x00;                //��ʱ��1Ϊģʽ0(16λ�Զ�����)
    TL1 = 0xd8;                 //���ò�������װֵ
    TH1 = 0xff;                 //115200 bps(65536-18432000/4/115200)
    TR1 = 1;                    //��ʱ��1��ʼ����
#else
    TMOD = 0x20;                //���ö�ʱ��1Ϊ8λ�Զ���װ��ģʽ
    AUXR = 0x40;                //��ʱ��1Ϊ1Tģʽ
    TH1 = TL1 = 0xfb;           //115200 bps(256 - 18432000/32/115200)
    TR1 = 1;
#endif
}

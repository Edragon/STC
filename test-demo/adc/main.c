

// P1.0 ch read ADC
// serial print channel 00 first, than print ADC value, for example 5V = 32 hex value
#include "reg51.h"
#include "intrins.h"


typedef unsigned char BYTE;
typedef unsigned int WORD;

#define     URMD    0           //0:ʹ�ö�ʱ��2��Ϊ�����ʷ�����
//1:ʹ�ö�ʱ��1��ģʽ0(16λ�Զ�����ģʽ)��Ϊ�����ʷ�����
//2:ʹ�ö�ʱ��1��ģʽ2(8λ�Զ�����ģʽ)��Ϊ�����ʷ�����

sfr T2H   = 0xd6;               //��ʱ��2��8λ
sfr T2L   = 0xd7;               //��ʱ��2��8λ



sfr  AUXR       =   0x8e;       //�����Ĵ���

sfr ADC_CONTR   =   0xBC;           //ADC���ƼĴ���
sfr ADC_RES     =   0xBD;           //ADC��8λ���
sfr ADC_LOW2    =   0xBE;           //ADC��2λ���
sfr P1ASF       =   0x9D;           //P1�ڵ�2���ܿ��ƼĴ���

#define ADC_POWER   0x80            //ADC��Դ����λ
#define ADC_FLAG    0x10            //ADC��ɱ�־
#define ADC_START   0x08            //ADC��ʼ����λ
#define ADC_SPEEDLL 0x00            //540��ʱ��
#define ADC_SPEEDL  0x20            //360��ʱ��
#define ADC_SPEEDH  0x40            //180��ʱ��
#define ADC_SPEEDHH 0x60            //90��ʱ��

void InitUart();
void InitADC();
void SendData(BYTE dat);
BYTE GetADCResult(BYTE ch);
void Delay(WORD n);
void ShowResult(BYTE ch);

void main()
{

    InitUart();                     //��ʼ������
    InitADC();                      //��ʼ��ADC
    while (1)
    {
        ShowResult(0);              //��ʾͨ��0
        Delay(100);
    }
}

/*----------------------------
����ADC�����PC
----------------------------*/
void ShowResult(BYTE ch)
{
    SendData(ch);                   //��ʾͨ����
    SendData(GetADCResult(ch));     //��ʾADC��8λ���

//    SendData(ADC_LOW2);           //��ʾ��2λ���
}

/*----------------------------
��ȡADC���
----------------------------*/
BYTE GetADCResult(BYTE ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //�ȴ�4��NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//�ȴ�ADCת�����
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    return ADC_RES;                 //����ADC���
}

/*----------------------------
��ʼ������
----------------------------*/
void InitUart()
{
    SCON = 0x5a;                //���ô���Ϊ8λ�ɱ䲨����
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

/*----------------------------
��ʼ��ADC
----------------------------*/
void InitADC()
{
    P1ASF = 0xff;                   //����P1��ΪAD��
    ADC_RES = 0;                    //�������Ĵ���
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    Delay(2);                       //ADC�ϵ粢��ʱ
}

/*----------------------------
���ʹ�������
----------------------------*/
void SendData(BYTE dat)
{
    while (!TI);                    //�ȴ�ǰһ�����ݷ������
    TI = 0;                         //������ͱ�־
    SBUF = dat;                     //���͵�ǰ����
}

/*----------------------------
�����ʱ
----------------------------*/
void Delay(WORD n)
{
    WORD x;

    while (n--)
    {
        x = 5000;
        while (x--);
    }
}


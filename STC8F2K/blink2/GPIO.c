#include "STC8F2K.h"
#include "intrins.h"

#include "bsp_delay.h"


//sfr     P2M0        =   0x96;
//sfr     P2M1        =   0x95;


void main()
{
    P2M0 = 0xff;                                //����P1.0~P1.7Ϊ�������ģʽ
    P2M1 = 0x00;
	
    P26 = 1;                                    //P0.0������ߵ�ƽ
    P26 = 0;                                    //P0.0������͵�ƽ



    while (1)
		{
			P26 = 1;   
			Delay1000ms();
			P26 = 0;   
			Delay1000ms();
			
		}
}


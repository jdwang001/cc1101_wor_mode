#include "mcu_config.h"


void ExterInterrupt(void)
{
//	IT0 = 1;	// �ⲿ�ж�0�½����ж�
//	EX0 = 1;	// �ⲿ�ж�0����
	IT1 = 1;	// �ⲿ�ж�1�½����ж�
	EX1 = 1;	// �ⲿ�ж�1����
}

void SpiInit(void)
{
    CSN=0;
    SCK=0;
    CSN=1;
}

/*****************************************************************************************
//��������CpuInit()
//���룺��
//�������
//����������SPI��ʼ������
/*****************************************************************************************/
void CpuInit(void)
{	 
	
    SpiInit();
    delay(50000);
    delay(50000);
		LED_D4 = ~LED_D4;
	 	delay(50000);
	  delay(50000);
		LED_D4 = ~LED_D4; 
	 	delay(50000);
	  delay(50000);
		LED_D4 = ~LED_D4; 
}


//*****************************************************************************************
//��������SpisendByte(INT8U dat)
//���룺���͵�����
//�������
//����������SPI����һ���ֽ�
//*****************************************************************************************
INT8U SpiTxRxByte(INT8U dat)
{
    INT8U i,temp;
    temp = 0;
    
    SCK = 0;
    for(i=0; i<8; i++)
    {
        if(dat & 0x80)
        {
            MOSI = 1;
        }
        else MOSI = 0;
        dat <<= 1;
        
        SCK = 1; 
        //for(i=0;i<2;i++) 
        //{
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        //}
        
        temp <<= 1;
        if(MISO)temp++; 
        SCK = 0;
        //for(i=0;i<2;i++) 
        //{
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        //}	
    }
    return temp;
}

/*AUXR �Ĵ��� �������ܼĴ��� */
void UART_init()
{
    SCON = 0x50;                 /* uart in mode 1 (8 bit), REN=1 */
    TMOD = TMOD | 0x20 ;         /* Timer 1 in mode 2 */
    TH1  = 0xFD;                 /* 9600 Bds at 11.059MHz */
    TL1  = 0xFD;                 /* 9600 Bds at 11.059MHz */
    TR1 = 1;                     /* Timer 1 run */
    
    ET1 =0; 
    ES=1;                // �����п��ж�
    EA =1;               // ��Ƭ���ж�����
}

void Log_printf(INT8U *p_log)
{
	ES = 0;	  			// ��ֹ�����ж�
	while(*p_log!=0)
	{
		SBUF = *p_log;
		while (!TI)	;
		TI=0;
		p_log++;
	}
	ES = 1;			    // �������ж�
}

void Usart_printf(INT8U *p_uart,INT8U num)
{
	ES = 0;
	while( (num--) != 0 )
	{
		SBUF = *p_uart;
		while (!TI)	;
		TI=0;
		p_uart++;
	}
	ES = 1;
}


//*****************************************************************************************
//��������delay(unsigned int s)
//���룺ʱ��
//�������
//������������ͨ͢ʱ,�ڲ���
//*****************************************************************************************		
void delay(unsigned int s)
{
    unsigned int i;
    //for(i=0;i<2;i++)
    //{
    for(i=0; i<s; i++);
    for(i=0; i<s; i++);
    //}
}

void ser()  interrupt 4   //�����ж�
{
    if(RI)								// RIΪ1�������������жϣ��ֶ�����
    {
        TxBuf[count]=SBUF;
  			if(TxBuf[0]!=0xAA)
				{
					count = 0;	
				}
				else
				{
					count++;
					if(count>10)		// ����11���ֽڣ������0~10
					{
						
						flag_rx = 0x55;
					}
				}
        RI=0;
    }
} 
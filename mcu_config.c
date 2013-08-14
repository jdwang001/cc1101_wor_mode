#include "mcu_config.h"


void ExterInterrupt(void)
{
//	IT0 = 1;	// 外部中断0下降沿中断
//	EX0 = 1;	// 外部中断0允许
	IT1 = 1;	// 外部中断1下降沿中断
	EX1 = 1;	// 外部中断1允许
}

void SpiInit(void)
{
    CSN=0;
    SCK=0;
    CSN=1;
}

/*****************************************************************************************
//函数名：CpuInit()
//输入：无
//输出：无
//功能描述：SPI初始化程序
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
//函数名：SpisendByte(INT8U dat)
//输入：发送的数据
//输出：无
//功能描述：SPI发送一个字节
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

/*AUXR 寄存器 辅助功能寄存器 */
void UART_init()
{
    SCON = 0x50;                 /* uart in mode 1 (8 bit), REN=1 */
    TMOD = TMOD | 0x20 ;         /* Timer 1 in mode 2 */
    TH1  = 0xFD;                 /* 9600 Bds at 11.059MHz */
    TL1  = 0xFD;                 /* 9600 Bds at 11.059MHz */
    TR1 = 1;                     /* Timer 1 run */
    
    ET1 =0; 
    ES=1;                // 允许串行口中断
    EA =1;               // 单片机中断允许
}

void Log_printf(INT8U *p_log)
{
	ES = 0;	  			// 禁止串口中断
	while(*p_log!=0)
	{
		SBUF = *p_log;
		while (!TI)	;
		TI=0;
		p_log++;
	}
	ES = 1;			    // 允许串口中断
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
//函数名：delay(unsigned int s)
//输入：时间
//输出：无
//功能描述：普通廷时,内部用
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

void ser()  interrupt 4   //串口中断
{
    if(RI)								// RI为1，向主机请求中断，手动清零
    {
        TxBuf[count]=SBUF;
  			if(TxBuf[0]!=0xAA)
				{
					count = 0;	
				}
				else
				{
					count++;
					if(count>10)		// 共计11个字节，数组从0~10
					{
						
						flag_rx = 0x55;
					}
				}
        RI=0;
    }
} 
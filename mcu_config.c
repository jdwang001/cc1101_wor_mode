	// Filename:	mcu_config.c
// Function:	rf底层驱动
// Author:		wzd
// Date:			2013年8月15日10:21:25


#include "mcu_config.h"


/*****************************************************************************************
//函数名：void IapIdle(void) 
//输入：无
//输出：无
//功能描述：禁止ISP/IAP/EEPROM 功能，是MCU处于安全状态
/*****************************************************************************************/
void IapIdle(void) 
{ 
  IAP_CONTR = 0;                        //Close IAP function 
  IAP_CMD = 0;                          //Clear command to standby 
  IAP_TRIG = 0;                         //Clear trigger register 
  IAP_ADDRH = 0x80;                     //Data ptr point to non-EEPROM area 
  IAP_ADDRL = 0;                        //Clear IAP address to prevent misuse 
} 

/*****************************************************************************************
//函数名：INT8U IapReadByte(INT16U addr) 
//输入：EEPROM地址
//输出：EEPROM地址中的数据
//功能描述：从EEPROM中读取一个字节，
/*****************************************************************************************/                                               
INT8U IapReadByte(INT16U addr)                                                
{         
  INT8U dat;                             //Data buffer                                                      
  IAP_CONTR = ENABLE_IAP;               //Open IAP function, and set wait time                                        
  IAP_CMD = CMD_READ;                   //Set ISP/IAP/EEPROM READ command                                   
  IAP_ADDRL = addr;                  //Set ISP/IAP/EEPROM address low                                  
  IAP_ADDRH = addr >> 8;                //Set ISP/IAP/EEPROM address high 
  IAP_TRIG = 0x5a;                     //Send trigger command1 (0x5a)                  
  IAP_TRIG = 0xa5;                      //Send trigger command2 (0xa5)               
  _nop_();                              //MCU will hold here until ISP/IAP/EEPROM                                        
                                         //operation complete 
  dat = IAP_DATA;                       //Read ISP/IAP/EEPROM data 
  IapIdle();                            //Close ISP/IAP/EEPROM function  
  return dat;                           //Return Flash data 
} 

/*****************************************************************************************
//函数名：void IapProgramByte(INT16U addr, INT8U dat)  
//输入：addr：EEPROM地址，dat：EEPROM地址中的数据
//输出：无
//功能描述：写入一个字节
/*****************************************************************************************/ 
void IapProgramByte(INT16U addr, INT8U dat) 
{ 
  IAP_CONTR = ENABLE_IAP;              //Open IAP function, and set wait time 
  IAP_CMD = CMD_PROGRAM;               //Set ISP/IAP/EEPROM PROGRAM command 
  IAP_ADDRL = addr;                    //Set ISP/IAP/EEPROM address low 
  IAP_ADDRH = addr >> 8;               //Set ISP/IAP/EEPROM address high 
  IAP_DATA = dat;                      //Write ISP/IAP/EEPROM data 
  IAP_TRIG = 0x5a;                     //Send trigger command1 (0x5a) 
  IAP_TRIG = 0xa5;                     //Send trigger command2 (0xa5) 
  _nop_();                             //MCU will hold here until ISP/IAP/EEPROM 
                                      //operation complete 
  IapIdle(); 
} 

/*****************************************************************************************
//函数名：void IapEraseSector(INT16U addr) 
//输入：addr：EEPROM地址
//输出：无
//功能描述：擦出一个扇区（512个字节为一个扇区）
/*****************************************************************************************/ 
void IapEraseSector(INT16U addr)                                 
{                                                                                                                                                         
  IAP_CONTR = ENABLE_IAP;              //Open IAP function, and set wait time                                       
  IAP_CMD = CMD_ERASE;                 //Set ISP/IAP/EEPROM ERASE command  扇区擦除                                 
  IAP_ADDRL = addr;                    //Set ISP/IAP/EEPROM address low                            
  IAP_ADDRH = addr >> 8;               //Set ISP/IAP/EEPROM address high                          
  IAP_TRIG = 0x5a;                    //Send trigger command1 (0x5a) 
  IAP_TRIG = 0xa5;                     //Send trigger command2 (0xa5)              
  _nop_();                           //MCU will hold here until ISP/IAP/EEPROM                                    
                                      //operation complete 
  IapIdle(); 
}

///*****************************************************************************************
////函数名：void IapEraseByte(INT16U addr,INT8U size)  
////输入：addr：EEPROM地址  size：擦除字节个数
////输出：1擦除成功 0擦除失败
////功能描述：按字节擦除  将每个字节编程为0xFF，而后应该可以直接写入数据。待验证
///*****************************************************************************************/  
//INT8U IapCheckEEPROM(INT16U addr,INT8U size)                                 
//{   
//	INT8U i;
//
//  for (i=0; i<size; i++)                         //Check whether all erase data is FF
//	{
//		if (IapReadByte(addr+i) != 0xff)
//			return 0;		
//	}
//	return 1;
//} 

/*****************************************************************************************
//函数名：void IapReadModelSn(INT16U addr,Model_Sn* sndata) 
//输入：addr：EEPROM地址 data: 读出的数据
//输出：无
//功能描述：读取EEPROM内的数据
/*****************************************************************************************/ 
void IapReadModelSn(INT16U addr,Module_Sn* sndata)
{
	INT8U i;
	for(i=0;i<2;i++)
	{
		sndata->Sn[i] = IapReadByte(addr+i);
	}
}

/*****************************************************************************************
//函数名：void IapRead2Array(INT16U addr,INT8U* arraydata,INT8U size) 
//输入：addr：EEPROM地址 arraydata: 数据存放到数组中 size：数据个数
//输出：无
//功能描述：读取EEPROM内的数据并存放到数组中
/*****************************************************************************************/ 
void IapRead2Array(INT16U addr,INT8U* arraydata,INT8U size)
{
	INT8U i;
	for(i=0;i<size;i++)
	{
		arraydata[i] = IapReadByte(addr+i);
	}
}

/*****************************************************************************************
//函数名：void Int1Init(void)
//输入：无
//输出：无
//功能描述：INT1中断初始化程序
/*****************************************************************************************/
void Int1Init(void)
{
//	IT0 = 1;	// 外部中断0下降沿中断
//	EX0 = 1;	// 外部中断0允许
		IT1 = 1;	// 外部中断1下降沿中断
		//INT1_ON;	// 外部中断1允许
}

/*****************************************************************************************
//函数名：void SpiInit(void)
//输入：无
//输出：无
//功能描述：SPI初始化程序
/*****************************************************************************************/
void SpiInit(void)
{
    CSN=0;
    SCK=0;
    CSN=1;
}

//*****************************************************************************************
//函数名：void UART_init()
//输入：无
//输出：无
//功能描述：配置串口通讯协议 9600 8 N 1	开启全局中断
//*****************************************************************************************
void UART_init(void)
{
    SCON = 0x50;								// uart in mode 1 (8 bit), REN=1
    TMOD|= 0x20;								// Timer 1 in mode 2
    TH1  = 0xFD;								// 9600 Bds at 11.059MHz
    TL1  = 0xFD;								// 9600 Bds at 11.059MHz
    TR1 = 1;										// Timer 1 run
    ET1 =0; 
    
    //WAKE_CLKO = 0x40;						 // 使能下降沿唤醒MCU，从掉电模式
    ES=1;												 // 开启串行口中断
}

//*****************************************************************************************
//函数名：void Log_printf(INT8U *p_log)
//输入：INT8U *p_log
//输出：*p_log指向的字符串
//功能描述：通过串口生成日志文件
//*****************************************************************************************
void Log_printf(INT8U *p_log)
{
	ES = 0;	  										// 禁止串口中断
	while(*p_log!=0)
	{
		SBUF = *p_log;
		while (!TI)	;
		TI=0;
		p_log++;
	}
	ES = 1;			    							// 允许串口中断
}

/*****************************************************************************************
//函数名：CpuInit()
//输入：无
//输出：无
//功能描述：SPI初始化程序，点亮LED4，初始化INT1
/*****************************************************************************************/
void CpuInit(void)
{	 
	
    //LED_R = ~LED_R;
    IapReadModelSn(MODEL_SN_ADDRESS,&g_module_id);
    IapReadModelSn(GATEWAY_ADDRESS,&g_gateway);
    Timer0_Init(1);
    Int1Init();
   	UART_init();   
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

//*****************************************************************************************
//函数名：void	Timer0_Init(INT16U ms)
//输入：定时Xms，注意别超过INT16范围
//输出：无
//功能描述：配置串口通讯协议 9600 8 N 1	开启全局中断
//*****************************************************************************************
void	Timer0_Init(INT16U ms)
{
		INT16U ms_count=0;
		
		ms_count = 65536-F_mcu*ms/12/1000;
		
		TMOD |= 0x01; 				// 设定定时器0为模式1
		TL0		=	ms_count;
		TH0		= ms_count>>8;
		
		//TR0		= 1;						// 开启定时器0
		ET0		= 1;					// 使能定时器0
}

//*****************************************************************************************
//函数名：void Usart_printf(INT8U *p_uart,INT8U num)
//输入：INT8U *p_uart，INT8U num
//输出：*p_uart指向的数组数据
//功能描述：发送num个数据
//*****************************************************************************************
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

//*****************************************************************************************
//函数名：void uart_isr()  interrupt 4
//输入：无
//输出：接收到的串口数据
//功能描述：中断接收串口数据帧
//*****************************************************************************************		
void uart_isr()  interrupt 4   //串口中断
{
    if(RI)								// RI为1，向主机请求中断，手动清零
    {
        TxBuf[g_count]=SBUF;
        
  			if(TxBuf[0]!=0xAA)
				{
					g_count = 0;	
				}
				else
				{
					g_count++;
					if(g_count>10)		// 共计11个字节，数组从0~10
					{
						
						g_rx_flag = 0x55;
					}
				}
        RI=0;
    }
}

//*****************************************************************************************
//函数名：void ser()  interrupt 4
//输入：无
//输出：接收到的串口数据
//功能描述：中断接收串口数据帧，定时2s
//*****************************************************************************************	
void timer0_isr()  interrupt 1   //Timer0中断
{
	if( 0x55 == g_search )
		Timer0_Init(10);
	else 
		Timer0_Init(1);
		
	timer++;
	if(timer>=1000)
	{
		//g_rx_timeout = 0x55;
		//g_rf_rx_flag = 0x00;
		//g_wor_flag = 0x00;				// 定时时间到，退出全速接收模式
		g_enter_rx = 0x00;
		//LED_D3 = ~LED_D3;
		timer = 0;
		TIMER0_OFF;								// 关闭定时器
	}
}

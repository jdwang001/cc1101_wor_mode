	// Filename:	mcu_config.c
// Function:	rf�ײ�����
// Author:		wzd
// Date:			2013��8��15��10:21:25


#include "mcu_config.h"


/*****************************************************************************************
//��������void IapIdle(void) 
//���룺��
//�������
//������������ֹISP/IAP/EEPROM ���ܣ���MCU���ڰ�ȫ״̬
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
//��������INT8U IapReadByte(INT16U addr) 
//���룺EEPROM��ַ
//�����EEPROM��ַ�е�����
//������������EEPROM�ж�ȡһ���ֽڣ�
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
//��������void IapProgramByte(INT16U addr, INT8U dat)  
//���룺addr��EEPROM��ַ��dat��EEPROM��ַ�е�����
//�������
//����������д��һ���ֽ�
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
//��������void IapEraseSector(INT16U addr) 
//���룺addr��EEPROM��ַ
//�������
//��������������һ��������512���ֽ�Ϊһ��������
/*****************************************************************************************/ 
void IapEraseSector(INT16U addr)                                 
{                                                                                                                                                         
  IAP_CONTR = ENABLE_IAP;              //Open IAP function, and set wait time                                       
  IAP_CMD = CMD_ERASE;                 //Set ISP/IAP/EEPROM ERASE command  ��������                                 
  IAP_ADDRL = addr;                    //Set ISP/IAP/EEPROM address low                            
  IAP_ADDRH = addr >> 8;               //Set ISP/IAP/EEPROM address high                          
  IAP_TRIG = 0x5a;                    //Send trigger command1 (0x5a) 
  IAP_TRIG = 0xa5;                     //Send trigger command2 (0xa5)              
  _nop_();                           //MCU will hold here until ISP/IAP/EEPROM                                    
                                      //operation complete 
  IapIdle(); 
}

///*****************************************************************************************
////��������void IapEraseByte(INT16U addr,INT8U size)  
////���룺addr��EEPROM��ַ  size�������ֽڸ���
////�����1�����ɹ� 0����ʧ��
////�������������ֽڲ���  ��ÿ���ֽڱ��Ϊ0xFF������Ӧ�ÿ���ֱ��д�����ݡ�����֤
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
//��������void IapReadModelSn(INT16U addr,Model_Sn* sndata) 
//���룺addr��EEPROM��ַ data: ����������
//�������
//������������ȡEEPROM�ڵ�����
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
//��������void IapRead2Array(INT16U addr,INT8U* arraydata,INT8U size) 
//���룺addr��EEPROM��ַ arraydata: ���ݴ�ŵ������� size�����ݸ���
//�������
//������������ȡEEPROM�ڵ����ݲ���ŵ�������
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
//��������void Int1Init(void)
//���룺��
//�������
//����������INT1�жϳ�ʼ������
/*****************************************************************************************/
void Int1Init(void)
{
//	IT0 = 1;	// �ⲿ�ж�0�½����ж�
//	EX0 = 1;	// �ⲿ�ж�0����
		IT1 = 1;	// �ⲿ�ж�1�½����ж�
		//INT1_ON;	// �ⲿ�ж�1����
}

/*****************************************************************************************
//��������void SpiInit(void)
//���룺��
//�������
//����������SPI��ʼ������
/*****************************************************************************************/
void SpiInit(void)
{
    CSN=0;
    SCK=0;
    CSN=1;
}

//*****************************************************************************************
//��������void UART_init()
//���룺��
//�������
//�������������ô���ͨѶЭ�� 9600 8 N 1	����ȫ���ж�
//*****************************************************************************************
void UART_init(void)
{
    SCON = 0x50;								// uart in mode 1 (8 bit), REN=1
    TMOD|= 0x20;								// Timer 1 in mode 2
    TH1  = 0xFD;								// 9600 Bds at 11.059MHz
    TL1  = 0xFD;								// 9600 Bds at 11.059MHz
    TR1 = 1;										// Timer 1 run
    ET1 =0; 
    
    //WAKE_CLKO = 0x40;						 // ʹ���½��ػ���MCU���ӵ���ģʽ
    ES=1;												 // �������п��ж�
}

//*****************************************************************************************
//��������void Log_printf(INT8U *p_log)
//���룺INT8U *p_log
//�����*p_logָ����ַ���
//����������ͨ������������־�ļ�
//*****************************************************************************************
void Log_printf(INT8U *p_log)
{
	ES = 0;	  										// ��ֹ�����ж�
	while(*p_log!=0)
	{
		SBUF = *p_log;
		while (!TI)	;
		TI=0;
		p_log++;
	}
	ES = 1;			    							// �������ж�
}

/*****************************************************************************************
//��������CpuInit()
//���룺��
//�������
//����������SPI��ʼ�����򣬵���LED4����ʼ��INT1
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

//*****************************************************************************************
//��������void	Timer0_Init(INT16U ms)
//���룺��ʱXms��ע��𳬹�INT16��Χ
//�������
//�������������ô���ͨѶЭ�� 9600 8 N 1	����ȫ���ж�
//*****************************************************************************************
void	Timer0_Init(INT16U ms)
{
		INT16U ms_count=0;
		
		ms_count = 65536-F_mcu*ms/12/1000;
		
		TMOD |= 0x01; 				// �趨��ʱ��0Ϊģʽ1
		TL0		=	ms_count;
		TH0		= ms_count>>8;
		
		//TR0		= 1;						// ������ʱ��0
		ET0		= 1;					// ʹ�ܶ�ʱ��0
}

//*****************************************************************************************
//��������void Usart_printf(INT8U *p_uart,INT8U num)
//���룺INT8U *p_uart��INT8U num
//�����*p_uartָ�����������
//��������������num������
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

//*****************************************************************************************
//��������void uart_isr()  interrupt 4
//���룺��
//��������յ��Ĵ�������
//�����������жϽ��մ�������֡
//*****************************************************************************************		
void uart_isr()  interrupt 4   //�����ж�
{
    if(RI)								// RIΪ1�������������жϣ��ֶ�����
    {
        TxBuf[g_count]=SBUF;
        
  			if(TxBuf[0]!=0xAA)
				{
					g_count = 0;	
				}
				else
				{
					g_count++;
					if(g_count>10)		// ����11���ֽڣ������0~10
					{
						
						g_rx_flag = 0x55;
					}
				}
        RI=0;
    }
}

//*****************************************************************************************
//��������void ser()  interrupt 4
//���룺��
//��������յ��Ĵ�������
//�����������жϽ��մ�������֡����ʱ2s
//*****************************************************************************************	
void timer0_isr()  interrupt 1   //Timer0�ж�
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
		//g_wor_flag = 0x00;				// ��ʱʱ�䵽���˳�ȫ�ٽ���ģʽ
		g_enter_rx = 0x00;
		//LED_D3 = ~LED_D3;
		timer = 0;
		TIMER0_OFF;								// �رն�ʱ��
	}
}

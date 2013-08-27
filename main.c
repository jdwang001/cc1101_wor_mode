// Project：capinfo-rf_route
// Filename:	mcu_config.c
// Function:	rf底层驱动
// Author:		wzd
// Date:			2013年8月15日10:21:25
//2013年8月24日9:42:18读状态寄存器需要进行修改
//INT8U halSpiReadStatus(INT8U addr) 
//{
//    INT8U value,temp;
//    temp = addr | READ_BURST;		//写入要读的状态寄存器的地址同时写入读命令
    

//#include "rf_config.h"
#include "rf_route.h"

Module_Sn g_module_id;
INT16U	timer = 0;
INT8U g_1s_counter=0,g_leng=0,g_count = 0,g_test_count=0;
INT8U g_wor_flag = 0x00,g_rx_flag = 0,g_rf_rx_flag = 0,g_rx_timeout = 0x00;
INT8U	WorCarry[2] = {0xFF,0xFF};
INT8U TxBuf[64];	 			// 11字节, 如果需要更长的数据包,请正确设置
INT8U RxBuf[64];
INT8U	Test[20] = "Send Packet";
//***************更多功率参数设置可详细参考DATACC1100英文文档中第48-49页的参数表******************
//INT8U PaTabel[8] = {0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04};  //-30dBm   功率最小
//INT8U PaTabel[8] = {0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60};  //0dBm
INT8U PaTabel[8] = {0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0};

const RF_SETTINGS rfSettings = 
{
    		0x00,		
        0x08,   // FSCTRL1   Frequency synthesizer control.   IF（中频 intermediate frequence） 晶振频率*val/2^10  = 203.125khz
        0x00,   // FSCTRL0   Frequency synthesizer control.		Fxtal/2^14 = 1.59k 
        
        0x10,   // FREQ2     Frequency control word, high byte. Fcarrier = 晶振频率*val/2^16 = 433Mhz  定义了中心频率
        0xA7,   // FREQ1     Frequency control word, middle byte.  
        0x62,   // FREQ0     Frequency control word, low byte.

				// 空中波特率 100kbaud，载波频率433MHZ 非Manchester编码 4Byte同步字(30/32同步字质量模式)，2-FSK
        // MDMCFG4指定了信道带宽
        // MDMCFG3指定了空中波特率
        // MDMCFG2指定滤波器，无线信号调制格式，4Byte前导码，4Byte同步字(30/32同步字质量模式)
        // 配置每个信道的带宽
        0x5B,   // MDMCFG4   Modem configuration.		CHANBW_E = 01	CHANBW_M =01 DRATE_E = 11	BW_channel = Fxosc/（8*（4+CHANBW_M）*2^CHANBW_E）= 325khz	
        0xF8,   // MDMCFG3   Modem configuration.		DRATE_M = 248 Rdata = 100kBaud  空中波特率
        0x03,   // MDMCFG2   Modem configuration.		DEM_DCFILT_OFF=0 MOD_FORMAT=000 MANCHESTER_EN=0（不使能）SYNC_MODE = 011(30/32sync word bits detected)
        0x22,   // MDMCFG1   Modem configuration.		FEC_EN=0(disable) NUM_PREAMBLE=010(4byte前导码) NU=00 CHANSPC_E=10  
        0xF8,   // MDMCFG0   Modem configuration.		不必关心
        
        // 0信道
        0x03,   // CHANNR    Channel number.				
        0x47,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
        
        // 少了两个配置，主要针对于wor模式的操作
        // MCSM2 WOR模式需要寄存器 需进行配置  RX_TIME[2:0]
        // MCSM1 RXOFF_MODE 接收数据包后，RF的状态
        
        0xB6,   // FREND1    Front end RX configuration.
        0x10,   // FREND0    Front end RX configuration.
        
       	// MCSM2
        // MCSM1		 RXOFF_MODE  TXOFF_MODE 同时配置为接收完成或发送完成后 切换到IDLE模式
        0x18,   // MCSM0     Main Radio Control State Machine configuration.   
        
        0x1D,   // FOCCFG    Frequency Offset Compensation Configuration.
        0x1C,   // BSCFG     Bit synchronization Configuration.
        0xC7,   // AGCCTRL2  AGC control.
        0x00,   // AGCCTRL1  AGC control.
        0xB2,   // AGCCTRL0  AGC control.
        
        0xEA,   // FSCAL3    Frequency synthesizer calibration.
        0x2A,   // FSCAL2    Frequency synthesizer calibration.
        0x00,   // FSCAL1    Frequency synthesizer calibration.
        0x11,   // FSCAL0    Frequency synthesizer calibration.
        
        0x59,   // FSTEST    Frequency synthesizer calibration.
        0x81,   // TEST2     Various test settings.
        0x35,   // TEST1     Various test settings.
        0x09,   // TEST0     Various test settings.
        0x0B,   // IOCFG2    GDO2 output pin configuration.
        0x06,   // IOCFG0    GDO0 output pin configuration. Refer to SmartRF?Studio User Manual for detailed pseudo register explanation.
        // 通过查询GDO0来确认是否接收到数据 接收到数据包置位 数据包末尾结束置位
        
        0x04,   // PKTCTRL1  Packet automation control.		APPEND_STATUS=1增加两个状态字节以及CRC_OK标记增加在数据包有效负载上 ADR_CHK=00无地址校验
        0x05,   // PKTCTRL0  Packet automation control.		NU=0 WHITE_DATA=1关闭数据白化 PKT_FORMAT=00(use FIFOs for RX and TX) NU=0 CRC_EN=1 LENGTH_CONFIG=01可变长度包模式
        0x00,   // ADDR      Device address.
        0xFF    // PKTLEN    Packet length.								可变数据包模式下，可发送的最大最大字节 定义为最大
};

void main()
{
    INT8U i=0;
    g_leng =55;  

    CpuInit();
    
    //验证读取的MCUSN
    Usart_printf(&g_module_id.Sn[0],1);
    Usart_printf(&g_module_id.Sn[1],1);
    
    POWER_UP_RESET_CC1100();
    halRfWriteRfSettings();
    halSpiWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8);
		CC1101_Setwor();


   	Log_printf("initialization ok\n");

    G_IT_ON;															// 开启单片机全局中断

    while (1)
    {
    	INT1_ON;														// 开外部中断
			PCON |= PD_ON;											// 从掉电模式唤醒后，程序从这行开市
			if( 0x55 == g_wor_flag )
			{
				while(g_wor_flag)
				halRfRxPacket(RxBuf);
			}
			halSpiStrobe(CCxxx0_SWORRST);      // 复位到 事件1
			halSpiStrobe(CCxxx0_SWOR);         // 启动WOR	
			//Log_printf("Exit pd\n");
    }	
    
//    while (1)
//    {
//    	//Log_printf("first pd\n");
//    	INT1_ON;												// 开外部中断
//			PCON |= PD_ON;									// 从掉电模式唤醒后，程序从这行开市
//			if( 0x55 == g_wor_flag )
//			{
//					g_wor_flag = 0x00;
//					CC1101_Worwakeup();
//					while( 0x55 == g_rf_rx_flag  )//&& 0x00 == g_rx_timeout
//					{
//						CC1101_EnterRx(RxBuf);
//					}
//			}
//			halSpiStrobe(CCxxx0_SWORRST);      //复位到 事件1
//			halSpiStrobe(CCxxx0_SWOR);         //启动WOR	
//			//Log_printf("Exit pd\n");
//    }						
}

//void main() 
// { 
//          INT16U i; 
//          INT8U eerom;
//          UART_init();
//          LED_D4 = ~LED_D4;
//          delay(50000);
//          //P1 = 0xfe;                                    //1111,1110 System Reset OK 
//          //Delay(10);                                    //Delay 
//          IapEraseSector(IAP_ADDRESS);                  //Erase current sector 
//          for (i=0; i<512; i++)                         //Check whether all sector data is FF 
//          { 
//                   if (IapReadByte(IAP_ADDRESS+i) != 0xff) 
//                   goto Error;                          //If error, break 
//          } 
//          //P1 = 0xfc;                                    //1111,1100 Erase successful 
//          //Delay(10);                                    //Delay 
// 
//          LED_D3 = ~LED_D3;
//          delay(50000);                                                                                   
//          for (i=0; i<512; i++)                         //Program 512 bytes data into data flash 
//                                                                               
//          {                                                                
//                   IapProgramByte(IAP_ADDRESS+i, (INT8U)i);         
//          }                                                    
//                                                                
//          //P1 = 0xf8;                                    //1111,1000 Program successful                                                                                                                                                                              
//          //Delay(10);                                    //Delay 
//          LED_D2 = ~LED_D2;
//          delay(50000);
//                                                             
//          for (i=0; i<512; i++)                        //Verify 512 bytes data 
//          {                             
//                   //eerom = IapReadByte(IAP_ADDRESS+i);
//                                     
//                   if ( (eerom = IapReadByte(IAP_ADDRESS+i)) != (INT8U)i)                                
//                   goto Error;                          //If error, break 
//                   Usart_printf(&eerom,1);    
//                              
//          }           
//          LED_D1 = ~LED_D1;
//          delay(50000);
//          //P1 = 0xf0;                                    //1111,0000 Verify successful 
//          while (1); 
//Error: 
//          //P1 &= 0x7f;                                   //0xxx,xxxx IAP operation fail 
//          Log_printf("Error EEPROM");
//          while (1); 
// } 
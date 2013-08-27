// Project��capinfo-rf_route
// Filename:	mcu_config.c
// Function:	rf�ײ�����
// Author:		wzd
// Date:			2013��8��15��10:21:25
//2013��8��24��9:42:18��״̬�Ĵ�����Ҫ�����޸�
//INT8U halSpiReadStatus(INT8U addr) 
//{
//    INT8U value,temp;
//    temp = addr | READ_BURST;		//д��Ҫ����״̬�Ĵ����ĵ�ַͬʱд�������
    

//#include "rf_config.h"
#include "rf_route.h"

Module_Sn g_module_id;
INT16U	timer = 0;
INT8U g_1s_counter=0,g_leng=0,g_count = 0,g_test_count=0;
INT8U g_wor_flag = 0x00,g_rx_flag = 0,g_rf_rx_flag = 0,g_rx_timeout = 0x00;
INT8U	WorCarry[2] = {0xFF,0xFF};
INT8U TxBuf[64];	 			// 11�ֽ�, �����Ҫ���������ݰ�,����ȷ����
INT8U RxBuf[64];
INT8U	Test[20] = "Send Packet";
//***************���๦�ʲ������ÿ���ϸ�ο�DATACC1100Ӣ���ĵ��е�48-49ҳ�Ĳ�����******************
//INT8U PaTabel[8] = {0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04};  //-30dBm   ������С
//INT8U PaTabel[8] = {0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60};  //0dBm
INT8U PaTabel[8] = {0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0};

const RF_SETTINGS rfSettings = 
{
    		0x00,		
        0x08,   // FSCTRL1   Frequency synthesizer control.   IF����Ƶ intermediate frequence�� ����Ƶ��*val/2^10  = 203.125khz
        0x00,   // FSCTRL0   Frequency synthesizer control.		Fxtal/2^14 = 1.59k 
        
        0x10,   // FREQ2     Frequency control word, high byte. Fcarrier = ����Ƶ��*val/2^16 = 433Mhz  ����������Ƶ��
        0xA7,   // FREQ1     Frequency control word, middle byte.  
        0x62,   // FREQ0     Frequency control word, low byte.

				// ���в����� 100kbaud���ز�Ƶ��433MHZ ��Manchester���� 4Byteͬ����(30/32ͬ��������ģʽ)��2-FSK
        // MDMCFG4ָ�����ŵ�����
        // MDMCFG3ָ���˿��в�����
        // MDMCFG2ָ���˲����������źŵ��Ƹ�ʽ��4Byteǰ���룬4Byteͬ����(30/32ͬ��������ģʽ)
        // ����ÿ���ŵ��Ĵ���
        0x5B,   // MDMCFG4   Modem configuration.		CHANBW_E = 01	CHANBW_M =01 DRATE_E = 11	BW_channel = Fxosc/��8*��4+CHANBW_M��*2^CHANBW_E��= 325khz	
        0xF8,   // MDMCFG3   Modem configuration.		DRATE_M = 248 Rdata = 100kBaud  ���в�����
        0x03,   // MDMCFG2   Modem configuration.		DEM_DCFILT_OFF=0 MOD_FORMAT=000 MANCHESTER_EN=0����ʹ�ܣ�SYNC_MODE = 011(30/32sync word bits detected)
        0x22,   // MDMCFG1   Modem configuration.		FEC_EN=0(disable) NUM_PREAMBLE=010(4byteǰ����) NU=00 CHANSPC_E=10  
        0xF8,   // MDMCFG0   Modem configuration.		���ع���
        
        // 0�ŵ�
        0x03,   // CHANNR    Channel number.				
        0x47,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
        
        // �����������ã���Ҫ�����worģʽ�Ĳ���
        // MCSM2 WORģʽ��Ҫ�Ĵ��� ���������  RX_TIME[2:0]
        // MCSM1 RXOFF_MODE �������ݰ���RF��״̬
        
        0xB6,   // FREND1    Front end RX configuration.
        0x10,   // FREND0    Front end RX configuration.
        
       	// MCSM2
        // MCSM1		 RXOFF_MODE  TXOFF_MODE ͬʱ����Ϊ������ɻ�����ɺ� �л���IDLEģʽ
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
        // ͨ����ѯGDO0��ȷ���Ƿ���յ����� ���յ����ݰ���λ ���ݰ�ĩβ������λ
        
        0x04,   // PKTCTRL1  Packet automation control.		APPEND_STATUS=1��������״̬�ֽ��Լ�CRC_OK������������ݰ���Ч������ ADR_CHK=00�޵�ַУ��
        0x05,   // PKTCTRL0  Packet automation control.		NU=0 WHITE_DATA=1�ر����ݰ׻� PKT_FORMAT=00(use FIFOs for RX and TX) NU=0 CRC_EN=1 LENGTH_CONFIG=01�ɱ䳤�Ȱ�ģʽ
        0x00,   // ADDR      Device address.
        0xFF    // PKTLEN    Packet length.								�ɱ����ݰ�ģʽ�£��ɷ��͵��������ֽ� ����Ϊ���
};

void main()
{
    INT8U i=0;
    g_leng =55;  

    CpuInit();
    
    //��֤��ȡ��MCUSN
    Usart_printf(&g_module_id.Sn[0],1);
    Usart_printf(&g_module_id.Sn[1],1);
    
    POWER_UP_RESET_CC1100();
    halRfWriteRfSettings();
    halSpiWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8);
		CC1101_Setwor();


   	Log_printf("initialization ok\n");

    G_IT_ON;															// ������Ƭ��ȫ���ж�

    while (1)
    {
    	INT1_ON;														// ���ⲿ�ж�
			PCON |= PD_ON;											// �ӵ���ģʽ���Ѻ󣬳�������п���
			if( 0x55 == g_wor_flag )
			{
				while(g_wor_flag)
				halRfRxPacket(RxBuf);
			}
			halSpiStrobe(CCxxx0_SWORRST);      // ��λ�� �¼�1
			halSpiStrobe(CCxxx0_SWOR);         // ����WOR	
			//Log_printf("Exit pd\n");
    }	
    
//    while (1)
//    {
//    	//Log_printf("first pd\n");
//    	INT1_ON;												// ���ⲿ�ж�
//			PCON |= PD_ON;									// �ӵ���ģʽ���Ѻ󣬳�������п���
//			if( 0x55 == g_wor_flag )
//			{
//					g_wor_flag = 0x00;
//					CC1101_Worwakeup();
//					while( 0x55 == g_rf_rx_flag  )//&& 0x00 == g_rx_timeout
//					{
//						CC1101_EnterRx(RxBuf);
//					}
//			}
//			halSpiStrobe(CCxxx0_SWORRST);      //��λ�� �¼�1
//			halSpiStrobe(CCxxx0_SWOR);         //����WOR	
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
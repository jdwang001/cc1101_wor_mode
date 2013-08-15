// Project��capinfo-rf_route
// Filename:	mcu_config.c
// Function:	rf�ײ�����
// Author:		wzd
// Date:			2013��8��15��10:21:25


#include "rf_config.h"

//INT16U GucCount;
INT16U g_module_id = 0;
INT8U g_1s_counter=0,g_leng=0,g_count = 0,g_test_count=0;
INT8U g_wor_flag = 0x00,g_rx_flag = 0;
INT8U TxBuf[64];	 			// 11�ֽ�, �����Ҫ���������ݰ�,����ȷ����
INT8U RxBuf[64];
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
        0x00,   // CHANNR    Channel number.				
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
        0x0c    // PKTLEN    Packet length.								�ɱ����ݰ�ģʽ�£��ɷ��͵��������ֽ�
};

void main()
{
    INT8U i,s_count=0;;
    g_leng =11; // Ԥ�ƽ��� 11 bytes 

    CpuInit();
    POWER_UP_RESET_CC1100();
    halRfWriteRfSettings();
    halSpiWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8);
		CC1101_Setwor();
		
		UART_init();
   	Log_printf("initialization ok\n");
   	Int1Init();

    while (1)
    {
    	//Log_printf("Enter pd\n");
			PCON |= PD_ON;
						
//			while ( g_rx_flag == 0x55 )
//			{
//				//g_rx_flag = 0x00;
//				
//				halRfReceivePacket(RxBuf,&g_leng);
//				//halRfSendPacket(TxBuf,count);	// Transmit Tx buffer data
//				LED_R=~LED_R;
//				//count = 0;
//			}
			


        /*
        if (GucCount++ > 1000)
        {
            GucCount = 0;
            halRfSendPacket(TxBuf,11);	// Transmit Tx buffer data
            LED_R = ~LED_R;
        }	 
        */
        	
        	//if(halRfReceivePacket(RxBuf,&leng))
//			while(GDO0)
//	        if(CC1101_Worwakeup(RxBuf,&leng))
//	        {							 	
//	            LED_B0 = ~LED_B0; 
	            //delay(5);
//	            ES=0;
//				
//	            for(i=0;i<s_count; i++)
//	            {   		  
//	                SBUF=RxBuf[i];
//	                while (!TI)	;
//	                TI=0;
//	            }
//	            ES=1;
	           	// GucCount = 0;
	            //delay(10000);
	            //halRfSendPacket(TxBuf,11);	// Transmit Tx buffer data
//	        }
        
//        if(halRfReceivePacket(RxBuf,&leng))
//        {							 	
//            LED_B0 = ~LED_B0; 
//            //delay(5);
//            ES=0;
//			
//            for(i=0;i<11; i++)
//            {   		  
//                SBUF=RxBuf[i];
//                while (!TI)	;
//                TI=0;
//            }
//            ES=1;
//
//           	// GucCount = 0;
//            //delay(10000);
//            //halRfSendPacket(TxBuf,11);	// Transmit Tx buffer data
//        }
        
    }
}
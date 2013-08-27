// Filename:	mcu_config.c
// Function:	rf�ײ�����
// Author:		wzd
// Date:			2013��8��15��10:21:25


#include "rf_config.h"

/*****************************************************************************************
//��������void halWait(INT16U timeout) 
//���룺��
//�������
//����������rf�����е���ʱ����
/*****************************************************************************************/
void halWait(INT16U timeout) 
{
    do 
    {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_(); 
    } 
    while (--timeout);
}

//*****************************************************************************************
//��������void RESET_CC1100(void)
//���룺��
//�������
//������������λCC1100
//*****************************************************************************************
void RESET_CC1100(void) 
{
    CSN = 0; 
    while (MISO);
    SpiTxRxByte(CCxxx0_SRES); 		//д�븴λ����
    while (MISO); 
    CSN = 1; 
}

//*****************************************************************************************
//��������void POWER_UP_RESET_CC1100(void) 
//���룺��
//�������
//�����������ϵ縴λCC1100
//*****************************************************************************************
void POWER_UP_RESET_CC1100(void) 
{
    CSN = 1; 
    halWait(10); 
    CSN = 0; 
    halWait(10); 
    CSN = 1; 
    halWait(410); 
    RESET_CC1100();   		//��λCC1100
}

//*****************************************************************************************
//��������void halSpiWriteReg(INT8U addr, INT8U value)
//���룺��ַ��������
//�������
//����������SPIд�Ĵ���
//*****************************************************************************************
void halSpiWriteReg(INT8U addr, INT8U value) 
{
    CSN = 0;
    while (MISO);
    SpiTxRxByte(addr);		//д��ַ
    SpiTxRxByte(value);		//д������
    CSN = 1;
}

//*****************************************************************************************
//��������void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//���룺��ַ��д�뻺������д�����
//�������
//����������SPI����д���üĴ���
//*****************************************************************************************
void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
{
    INT8U i, temp;
    temp = addr | WRITE_BURST;			//��λͻ��λ������ͻ����ȡ
    CSN = 0;												//��ʼͻ����ȡ
    while (MISO);
    SpiTxRxByte(temp);
    for (i = 0; i < count; i++)
    {
        SpiTxRxByte(buffer[i]);
    }
    CSN = 1;												//ͨ���ø�CSN����ֹͻ����ȡ
}

//*****************************************************************************************
//��������void halSpiStrobe(INT8U strobe)
//���룺����
//�������
//����������SPIд����
//*****************************************************************************************
void halSpiStrobe(INT8U strobe) 
{
    CSN = 0;
    while (MISO);
    SpiTxRxByte(strobe);		//д������
    CSN = 1;
}

//*****************************************************************************************
//��������INT8U halSpiReadReg(INT8U addr)
//���룺��ַ
//������üĴ�����������
//����������SPI���Ĵ���
//*****************************************************************************************
INT8U halSpiReadReg(INT8U addr) 
{
    INT8U temp, value;
    temp = addr|READ_SINGLE;//���Ĵ�������
    CSN = 0;
    while (MISO);
    SpiTxRxByte(temp);
    value = SpiTxRxByte(0);
    CSN = 1;
    return value;
}

//*****************************************************************************************
//��������void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//���룺��ַ���������ݺ��ݴ�Ļ��������������ø���
//�������
//����������SPI����д���üĴ���
//*****************************************************************************************
void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
{
    INT8U i,temp;
    temp = addr | READ_BURST;		//д��Ҫ�������üĴ�����ַ�Ͷ�����
    CSN = 0;
    while (MISO);
    SpiTxRxByte(temp);   
    for (i = 0; i < count; i++) 
    {
        buffer[i] = SpiTxRxByte(0);
    }
    CSN = 1;
}

//*****************************************************************************************
//��������INT8U halSpiReadReg(INT8U addr)
//���룺��ַ
//�������״̬�Ĵ�����ǰֵ
//����������SPI��״̬�Ĵ���
//*****************************************************************************************
INT8U halSpiReadStatus(INT8U addr) 
{
    INT8U value,temp;
    temp = addr | READ_BURST;		//д��Ҫ����״̬�Ĵ����ĵ�ַͬʱд�������
    CSN = 0;
    while (MISO);
    SpiTxRxByte(temp);
    value = SpiTxRxByte(0);
    CSN = 1;
    return value;
}
//INT8U halSpiReadStatus(INT8U addr) 
//{
//    INT8U value,temp;
//    temp = addr | READ_SINGLE;		//д��Ҫ����״̬�Ĵ����ĵ�ַͬʱд�������
//    CSN = 0;
//    while (MISO);
//    SpiTxRxByte(temp);
//    value = SpiTxRxByte(0);
//    CSN = 1;
//    return value;
//}

//*****************************************************************************************
//��������void halRfWriteRfSettings(RF_SETTINGS *pRfSettings)
//���룺��
//�������
//��������������CC1100�ļĴ���
//*****************************************************************************************
void halRfWriteRfSettings(void) 
{
    halSpiWriteReg(CCxxx0_IOCFG2,   rfSettings.IOCFG2);
    halSpiWriteReg(CCxxx0_IOCFG0,   rfSettings.IOCFG0); 
    
    halSpiWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL2);//���Ѽӵ�
    // Write register settings
    halSpiWriteReg(CCxxx0_FSCTRL1,  rfSettings.FSCTRL1);
    halSpiWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL0);
    halSpiWriteReg(CCxxx0_FREQ2,    rfSettings.FREQ2);
    halSpiWriteReg(CCxxx0_FREQ1,    rfSettings.FREQ1);
    halSpiWriteReg(CCxxx0_FREQ0,    rfSettings.FREQ0);
    halSpiWriteReg(CCxxx0_MDMCFG4,  rfSettings.MDMCFG4);
    halSpiWriteReg(CCxxx0_MDMCFG3,  rfSettings.MDMCFG3);
    halSpiWriteReg(CCxxx0_MDMCFG2,  rfSettings.MDMCFG2);
    halSpiWriteReg(CCxxx0_MDMCFG1,  rfSettings.MDMCFG1);
    halSpiWriteReg(CCxxx0_MDMCFG0,  rfSettings.MDMCFG0);
    halSpiWriteReg(CCxxx0_CHANNR,   rfSettings.CHANNR);
    halSpiWriteReg(CCxxx0_DEVIATN,  rfSettings.DEVIATN);
    halSpiWriteReg(CCxxx0_FREND1,   rfSettings.FREND1);
    halSpiWriteReg(CCxxx0_FREND0,   rfSettings.FREND0);
    halSpiWriteReg(CCxxx0_MCSM0 ,   rfSettings.MCSM0 );
    halSpiWriteReg(CCxxx0_FOCCFG,   rfSettings.FOCCFG);
    halSpiWriteReg(CCxxx0_BSCFG,    rfSettings.BSCFG);
    halSpiWriteReg(CCxxx0_AGCCTRL2, rfSettings.AGCCTRL2);
    halSpiWriteReg(CCxxx0_AGCCTRL1, rfSettings.AGCCTRL1);
    halSpiWriteReg(CCxxx0_AGCCTRL0, rfSettings.AGCCTRL0);
    halSpiWriteReg(CCxxx0_FSCAL3,   rfSettings.FSCAL3);
    halSpiWriteReg(CCxxx0_FSCAL2,   rfSettings.FSCAL2);
    halSpiWriteReg(CCxxx0_FSCAL1,   rfSettings.FSCAL1);
    halSpiWriteReg(CCxxx0_FSCAL0,   rfSettings.FSCAL0);
    halSpiWriteReg(CCxxx0_FSTEST,   rfSettings.FSTEST);
    halSpiWriteReg(CCxxx0_TEST2,    rfSettings.TEST2);
    halSpiWriteReg(CCxxx0_TEST1,    rfSettings.TEST1);
    halSpiWriteReg(CCxxx0_TEST0,    rfSettings.TEST0);
   
    halSpiWriteReg(CCxxx0_PKTCTRL1, rfSettings.PKTCTRL1);
    halSpiWriteReg(CCxxx0_PKTCTRL0, rfSettings.PKTCTRL0);
    halSpiWriteReg(CCxxx0_ADDR,     rfSettings.ADDR);
    halSpiWriteReg(CCxxx0_PKTLEN,   rfSettings.PKTLEN);
}

//*****************************************************************************************
//��������void halRfSendPacket(INT8U *txBuffer, INT8U size)
//���룺���͵Ļ��������������ݸ���
//�������
//����������CC1101����һ������
//*****************************************************************************************
void halRfSendPacket(INT8U *txBuffer, INT8U size) 
{
		// ������IOCFG0.GDO0_CFG=0x06 ����/���յ�ͬ����ʱ��λ���������ݰ���ĩβȡ����λ
		//INT1_OFF;
		// �״γ�ʼ���������ɱ����ݰ����ȣ������ֽڱ�����д�룬û�п�����ַʶ�𣬵ڶ����ֽ�����д��ADR
    halSpiWriteReg(CCxxx0_TXFIFO, size);
    halSpiWriteBurstReg(CCxxx0_TXFIFO, txBuffer, size);	//д��Ҫ���͵�����
    
    halSpiStrobe(CCxxx0_STX);		//���뷢��ģʽ��������
    
    // Wait for GDO0 to be set -> sync transmitted
    while (!GDO0);
    // Wait for GDO0 to be cleared -> end of packet �����л���IDLEģʽ
    while (GDO0);
    // ����ָ����TX_FIFO
    halSpiStrobe(CCxxx0_SFTX);
    //INT1_ON;
}

//*****************************************************************************************
//��������INT8U halRfReceivePacket(INT8U *rxBuffer, INT8U *length) 
//���룺INT8U *rxBuffer ���߽������ݣ�INT8U *length �����ֽ���
//��������ؽ��յ����ֽ���
//����������CC1101�����յ����������ݴ洢��*rxBufferָ���������
//*****************************************************************************************
INT8U halRfReceivePacket(INT8U *rxBuffer, INT8U *length) 
{
    INT8U status[2];
    INT8U packetLength;
    INT8U i=(*length)*4;  // �������Ҫ����datarate��length������
    
    //Log_printf("Enter rx  ");
    halSpiStrobe(CCxxx0_SRX);		//�������״̬
    //delay(5);
    //while (!GDO1);
    //while (GDO1);
    delay(20);
    while (GDO0)
    {
        delay(20);
        --i;
        if(i<1)
        	{
        		//Log_printf("Enter rx time out  ");
            return 0; 	 
           }   
    }	 
    
    // CCxxx0_RXBYTES RX_FIFO���ֽ���
    if ((halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) //����ӵ��ֽ�����Ϊ0
    {
        packetLength = halSpiReadReg(CCxxx0_RXFIFO);//������һ���ֽڣ����ֽ�Ϊ��֡���ݳ���
	
				//���Գ���
				//Usart_printf(&packetLength,1);

        if (packetLength <= *length) 		//�����Ҫ����Ч���ݳ���С�ڵ��ڽ��յ������ݰ��ĳ���
        {
        	//Log_printf("rx data  ");
            halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength); //�������н��յ�������
            *length = packetLength;				//�ѽ������ݳ��ȵ��޸�Ϊ��ǰ���ݵĳ���
            
            Usart_printf(rxBuffer,packetLength);

            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 	//����CRCУ��λ
            halSpiStrobe(CCxxx0_SFRX);		//��ϴ���ջ�����
            return (status[1] & CRC_OK);			//���У��ɹ����ؽ��ճɹ�
        }
        else 
        {
        	//Log_printf("length big  ");
            *length = packetLength;
            halSpiStrobe(CCxxx0_SFRX);		//��ϴ���ջ�����
            return 0;
        }
    } 
    else
    	{
    		//Log_printf("packet 0  ");
        return 0;
      }
}

//*****************************************************************************************
//��������INT8U CC1101_EnterRx(INT8U *rxBuffer) 
//���룺INT8U *rxBuffer ���߽�������
//��������ؽ��յ����ֽ���
//��������������ȫ�ٴ���ģʽ����ȡ����֡
//*****************************************************************************************
//INT8U CC1101_EnterRx(INT8U *rxBuffer) 
//{
//    INT8U status[2],wor_data[2];
//    INT8U packetLength=0;
//
//    halSpiStrobe(CCxxx0_SRX);		//�������״̬ 
//    if ((halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) //����ӵ��ֽ�����Ϊ0
//    {
//        packetLength = halSpiReadReg(CCxxx0_RXFIFO);//������һ���ֽڣ����ֽ�Ϊ��֡���ݳ���
//				//delay(100);
//				Usart_printf(&packetLength,1);
//		
//				if ( packetLength > 2 )	
//		    {		
//		        halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength); //�������н��յ�������
//		        //*length = packetLength;				//�ѽ������ݳ��ȵ��޸�Ϊ��ǰ���ݵĳ���
//		        //Usart_printf(rxBuffer,packetLength);
//		        // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
//		        halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 	//����CRCУ��λ
//		        halSpiStrobe(CCxxx0_SFRX);		//��ϴ���ջ�����
//		        //return (status[1] & CRC_OK);			//���У��ɹ����ؽ��ճɹ�
//		        if( status[1] & CRC_OK )
//		        {
//		        	Usart_printf(rxBuffer,packetLength);
//		        	// �����������������������������˴����ӽ�������У�飬�ж��Ƿ�Ϊ�������ݵ�֡����������·���߼����д���
//		        	// ����������������������������
//		        	return packetLength;
//		        }
//		        else
//		        {	
//		        	return 0;
//						}
//		    }
//
//        if ( packetLength == 2 ) 																		//�����Ҫ����Ч���ݳ��ȵ��ڽ��յ������ݰ��ĳ���
//        {
//            halSpiReadBurstReg(CCxxx0_RXFIFO, wor_data, packetLength); 	//�������н��յ�������
//            
//						if ( (wor_data[0]==BROADCAST && wor_data[1]==BROADCAST) || ( (wor_data[0]==g_module_id) && (wor_data[1]==g_module_id>>8) ) )
//						{
//	            //Log_printf("Enter Rx\n");
//							Usart_printf(wor_data,2);
//	            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
//	            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 						//����CRCУ��λ
//	            halSpiStrobe(CCxxx0_SFRX);																//��ϴ���ջ�����
//	            if(status[1] & CRC_OK)
//	            {
//								halSpiStrobe(CCxxx0_SFRX); 
//								g_rf_rx_flag = 0x55;																	// ��־��λ�󣬽���ȫ�ٽ���ģʽ
//								Timer0_Init(1);
//								TIMER0_ON;																						// ������ʱ��0 ��ʱ1s																				// ������ʱ��0 ��ʱ1s
//	            	return packetLength;
//	            }
//	            else
//	            {
//	            	
//	            	Log_printf("CRC error\n");
//	          	}							
//						}
//						else
//						{
//							Usart_printf(wor_data,2);
//							Log_printf("  why Not me\n");	
//						}
//      	}
//
//        //*length = packetLength;
//        halSpiStrobe(CCxxx0_SFRX);		//��ϴ���ջ�����
//        return 0;
//    } 
//    else
//        return 0;
//}

INT8U halRfRxPacket(INT8U *rxBuffer) 
{
    INT8U status[2],wor_data[2];
    INT8U packetLength=0;
    //INT8U i=(*length)*4;  																					// �������Ҫ����datarate��length������

    halSpiStrobe(CCxxx0_SRX);																					// �������״̬

    if ((halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) 				// ����ӵ��ֽ�����Ϊ0
    {
        packetLength = halSpiReadReg(CCxxx0_RXFIFO);									// ������һ���ֽڣ����ֽ�Ϊ��֡���ݳ���
				delay(100);
				//Usart_printf(&packetLength,1);
		
		if ( packetLength > 2 )	
	    {		
	        halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength); 	// �������н��յ�������
	        //*length = packetLength;																		// �ѽ������ݳ��ȵ��޸�Ϊ��ǰ���ݵĳ���
	        //Usart_printf(rxBuffer,packetLength);
	        // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
	        halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 							// ����CRCУ��λ
	        halSpiStrobe(CCxxx0_SFRX);																	// ��ϴ���ջ�����
	        //return (status[1] & CRC_OK);															// ���У��ɹ����ؽ��ճɹ�
	        if( status[1] & CRC_OK )
	        {
						//TIMER0_OFF;																								// �رն�ʱ��
	        	//g_wor_flag = 0x00;																				// ���յ����ݺ��˳�ȫ�ٽ���ģʽ
	        	Usart_printf(rxBuffer,packetLength);
	        	halRfSendPacket(Test, 13);																// Ӧ����Ϣ
	        	
						LED_D3 = ~LED_D3;
	        	return packetLength;
	        }
	        else
	        {	
	        	Log_printf("3 and crc\n");
	        	return 0;
					}
	    }

      if (packetLength == 2) 																					// �����Ҫ����Ч���ݳ��ȵ��ڽ��յ������ݰ��ĳ���
      {
          halSpiReadBurstReg(CCxxx0_RXFIFO, wor_data, packetLength); 	// �������н��յ�������
					if ( (wor_data[0]==BROADCAST && wor_data[1]==BROADCAST) || ( (wor_data[0]==g_module_id.Sn[0]) && (wor_data[1]==g_module_id.Sn[1]) ) )
					//if ( (wor_data[0]==0x55 && wor_data[1]==0xAA) || (wor_data[0]==g_module_id && wor_data[1]==g_module_id>>8) )
					{
            //Log_printf("Enter wor\n");
            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 						// ����CRCУ��λ
            halSpiStrobe(CCxxx0_SFRX);																// ��ϴ���ջ�����
            if(status[1] & CRC_OK)
            {
							halSpiStrobe(CCxxx0_SFRX);
							timer = 0; 
							Timer0_Init(1);
							TIMER0_ON;																							// ���յ����Ѳ�����ˢ�¶�ʱʱ�䣬��ʱ1s
							//Log_printf("RX\n");
							//Usart_printf(wor_data,packetLength);
            	return packetLength;
            }
            else
            {
            	Log_printf("CRC error\n");
          	}							
					}
					else
					{
						Usart_printf(wor_data,2);
						Log_printf("  why Not me\n");	
					}
    	}        

        //*length = packetLength;
        //Log_printf("not 2 3\n");
        halSpiStrobe(CCxxx0_SFRX);		//��ϴ���ջ�����
        return 0;
    } 
    else
        return 0;
}

//INT8U CC1101_EnterRx(INT8U *rxBuffer) 
//{
//    INT8U status[2],wor_data[2];
//    INT8U packetLength=0;
//
//    halSpiStrobe(CCxxx0_SRX);		//�������״̬ 
//    if ((halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) //����ӵ��ֽ�����Ϊ0
//    {
//        packetLength = halSpiReadReg(CCxxx0_RXFIFO);//������һ���ֽڣ����ֽ�Ϊ��֡���ݳ���
//				//delay(100);
//				Usart_printf(&packetLength,1);
//		
//				if ( packetLength > 2 )	
//		    {		
//		        halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength); //�������н��յ�������
//		        //*length = packetLength;				//�ѽ������ݳ��ȵ��޸�Ϊ��ǰ���ݵĳ���
//		        //Usart_printf(rxBuffer,packetLength);
//		        // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
//		        halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 	//����CRCУ��λ
//		        halSpiStrobe(CCxxx0_SFRX);		//��ϴ���ջ�����
//		        //return (status[1] & CRC_OK);			//���У��ɹ����ؽ��ճɹ�
//		        if( status[1] & CRC_OK )
//		        {
//		        	Usart_printf(rxBuffer,packetLength);
//		        	g_rf_rx_flag = 0x00;
//		        	// �����������������������������˴����ӽ�������У�飬�ж��Ƿ�Ϊ�������ݵ�֡����������·���߼����д���
//		        	// ����������������������������
//		        	return packetLength;
//		        }
//		        else
//		        {	
//		        	return 0;
//						}
//		    }
//
//        if ( packetLength == 2 ) 																		//�����Ҫ����Ч���ݳ��ȵ��ڽ��յ������ݰ��ĳ���
//        {
//            halSpiReadBurstReg(CCxxx0_RXFIFO, wor_data, packetLength); 	//�������н��յ�������
//            
//						if ( (wor_data[0]==BROADCAST && wor_data[1]==BROADCAST) || ( (wor_data[0]==g_module_id) && (wor_data[1]==g_module_id>>8) ) )
//						{
//	            //Log_printf("Enter Rx\n");
//							Usart_printf(wor_data,2);
//	            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
//	            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 						//����CRCУ��λ
//	            halSpiStrobe(CCxxx0_SFRX);																//��ϴ���ջ�����
//	            if(status[1] & CRC_OK)
//	            {
//								halSpiStrobe(CCxxx0_SFRX); 
//								g_rf_rx_flag = 0x55;																	// ��־��λ�󣬽���ȫ�ٽ���ģʽ
//								//Timer0_Init(1);
//								//TIMER0_ON;																						// ������ʱ��0 ��ʱ1s																				// ������ʱ��0 ��ʱ1s
//	            	return packetLength;
//	            }
//	            else
//	            {
//	            	
//	            	Log_printf("CRC error\n");
//	          	}							
//						}
//						else
//						{
//							Usart_printf(wor_data,2);
//							Log_printf("  why Not me\n");	
//						}
//      	}
//
//        //*length = packetLength;
//        halSpiStrobe(CCxxx0_SFRX);		//��ϴ���ջ�����
//        return 0;
//    } 
//    else
//        return 0;
//}

//*****************************************************************************************
//��������INT8U CC1101_Worwakeup(INT8U *rxBuffer, INT8U *length)  
//���룺INT8U *rxBuffer ���߽������ݣ�INT8U *length �����ֽ���
//��������ؽ��յ����ֽ���
//����������CC1101�����յ����������ݴ洢��*rxBufferָ���������
//*****************************************************************************************
//INT8U CC1101_Worwakeup(void) 
//{
//    INT8U wor_data[2],status[2];
//    INT8U packetLength;
//
//    // ��⵽���ݰ��󣬽������ģʽ
//    halSpiStrobe(CCxxx0_SRX);
//    // CCxxx0_RXBYTES RX_FIFO���ֽ���
//    if ( (halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO) ) 						//����ӵ��ֽ�����Ϊ0
//    {
//        	packetLength = halSpiReadReg(CCxxx0_RXFIFO);										//������һ���ֽڣ����ֽ�Ϊ��֡���ݳ���
//					//Usart_printf(&packetLength,1);
//				
//	        if (packetLength == 2) 																		//�����Ҫ����Ч���ݳ��ȵ��ڽ��յ������ݰ��ĳ���
//	        {
//	            halSpiReadBurstReg(CCxxx0_RXFIFO, wor_data, packetLength); 	//�������н��յ�������
//	            //*length = packetLength;																		//�ѽ������ݳ��ȵ��޸�Ϊ��ǰ���ݵĳ���
//							if ( (wor_data[0]==BROADCAST && wor_data[1]==BROADCAST) || ( (wor_data[0]==g_module_id) && (wor_data[1]==g_module_id>>8) ) )
//							//if ( (wor_data[0]==0x55 && wor_data[1]==0xAA) || (wor_data[0]==g_module_id && wor_data[1]==g_module_id>>8) )
//							{
//		            //Log_printf("Enter wor\n");
//		            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
//		            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 						//����CRCУ��λ
//		            halSpiStrobe(CCxxx0_SFRX);																//��ϴ���ջ�����
//		            if(status[1] & CRC_OK)
//		            {
//		            	
//									halSpiStrobe(CCxxx0_SFRX); 
//									g_rf_rx_flag = 0x55;																	// ��־��λ�󣬽���ȫ�ٽ���ģʽ
//									//g_rx_timeout = 0x00;
//									Timer0_Init(1);
//									TIMER0_ON;																						// ������ʱ��0 ��ʱ1s
//									//Log_printf("Init Timer0\n");
//		            	return packetLength;
//		            }
//		            else
//		            {
//		            	
//		            	Log_printf("CRC error\n");
//		          	}							
//							}
//							else
//							{
//								Usart_printf(wor_data,2);
//								Log_printf("  why Not me\n");	
//							}
//        	}
//	        else 
//	        {
//	        		Log_printf("Length!=2\n");
//	        }
//    } 
//    else
//    {
//    	//Log_printf("Not packet\n");
//    }
//    
//		halSpiStrobe(CCxxx0_SFRX); 
//		halSpiStrobe(CCxxx0_SWORRST);      																	//��λ�� �¼�1
//		halSpiStrobe(CCxxx0_SWOR);         																	//����WOR	
//		INT1_ON;												//2013��8��15��16:22:59
//    return 0;
//}

INT8U CC1101_Worwakeup(void) 
{
    INT8U wor_data[2],status[2];
    INT8U packetLength;

    // ��⵽���ݰ��󣬽������ģʽ
    halSpiStrobe(CCxxx0_SRX);
    // CCxxx0_RXBYTES RX_FIFO���ֽ���
    if ( (halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO) ) 						//����ӵ��ֽ�����Ϊ0
    {
        	packetLength = halSpiReadReg(CCxxx0_RXFIFO);										//������һ���ֽڣ����ֽ�Ϊ��֡���ݳ���
					//Usart_printf(&packetLength,1);
				
	        if (packetLength == 2) 																		//�����Ҫ����Ч���ݳ��ȵ��ڽ��յ������ݰ��ĳ���
	        {
	            halSpiReadBurstReg(CCxxx0_RXFIFO, wor_data, packetLength); 	//�������н��յ�������
	            //*length = packetLength;																		//�ѽ������ݳ��ȵ��޸�Ϊ��ǰ���ݵĳ���
							if ( (wor_data[0]==BROADCAST && wor_data[1]==BROADCAST) || ( (wor_data[0]==g_module_id.Sn[0]) && (wor_data[1]==g_module_id.Sn[1]) ) )
							//if ( (wor_data[0]==0x55 && wor_data[1]==0xAA) || (wor_data[0]==g_module_id && wor_data[1]==g_module_id>>8) )
							{
		            //Log_printf("Enter wor\n");
		            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
		            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 						//����CRCУ��λ
		            halSpiStrobe(CCxxx0_SFRX);																//��ϴ���ջ�����
		            if(status[1] & CRC_OK)
		            {
		            	
									halSpiStrobe(CCxxx0_SFRX); 
									g_wor_flag = 0x55;																		// Wor������λ��֮�����ȫ�ٽ���ģʽ
									
									timer = 0;
									Timer0_Init(1);
									TIMER0_ON;																						// ������ʱ��0 ��ʱ1s
		            	
		            	return packetLength;
		            }
		            else
		            {
		            	Log_printf("CRC error\n");
		          	}							
							}
							else
							{
								Usart_printf(wor_data,2);
								Log_printf("  why Not me\n");	
							}
        	}
	        else 
	        {
	        		Log_printf("Length!=2\n");
	        		Usart_printf(&packetLength,1);
	        }
    } 
    else
    {
    	Log_printf("Not packet\n");
    }
    
		halSpiStrobe(CCxxx0_SFRX); 
		halSpiStrobe(CCxxx0_SWORRST);      																	//��λ�� �¼�1
		halSpiStrobe(CCxxx0_SWOR);         																	//����WOR	
		INT1_ON;												//2013��8��15��16:22:59
    return 0;
}

//*****************************************************************************************
//��������INT8U CC1101_Setwor(void)  
//���룺��
//�������
//��������������rf����ģʽ���� EVENT0=300ms  rx_timeout=37.5ms
//***************************************************************************************** 
INT8U CC1101_Setwor(void)
{
 	halSpiStrobe(CCxxx0_SIDLE); //����ģʽ
  // MCSM2.RX_TIME = 001b
  //	Rx_timeout = ��10400*3.6058��us = 37.5ms
  // => Rx_timeout = (EVENT0*C(RX_TIME, WOR_RES))*26/x   x����Ƶ��
  halSpiWriteReg(CCxxx0_MCSM2, 0x00); 			//��Ҫ����wor��Rx�Ľ���ʱ�ޣ�Rx��duty cycle
  // Enable automatic FS calibration when going from IDLE to RX/TX/FSTXON (in between EVENT0 and EVENT1)
  //��TX,RX�� �Զ�У׼   XSOCʱ�� (10) 149-155uS
  //MSCM1 Ĭ��ֵ RXOFF_MODE �� TXOFF_MODE ���յ����ݰ��� ת����IDLEģʽ
  halSpiWriteReg(CCxxx0_MCSM0, 0x18);                  //У׼ FS_AUTOCAL[1:0]  01    ��IDLEת��TX OR RXģʽʱ
  
	//����EVENT0ʱ��Ϊ300ms��Txһ֡���Ѳ�12���ֽ�(960us)
  halSpiWriteReg(CCxxx0_WOREVT1, 0x28);        			// High byte Event0 timeout
  halSpiWriteReg(CCxxx0_WOREVT0, 0xA0);             // Low byte Event0 timeout.
 
  // ���� WOR RCosc У׼
  // ��Ϊ�������ߺ�ֻʹ��RCƵ������,RC�ܻ������¶�Ӱ��ϴ�,���Ա���һ��ʱ�����WOR���Ѻ�����У׼һ��ʱ��.
  // ��WORû����֮ǰ RC�����������
  // tEvent1 ʱ������Ϊ���,���� T_event1 ~ 1.4 ms
  halSpiWriteReg(CCxxx0_WORCTRL, 0x78);             		//tEvent1 =0111 ���յ����ʱ�� 
  //--RC_CAL =1 �Զ�У׼
  //halWait(30);                                                //�ȴ�У׼���
  //halSpiWriteReg(CCxxx0_WORCTRL, 0x70 | WOR_RES);           // tEvent1 =0111 �� 48 (1.333-1.385 ms)
  // RC_CAL =0
 
  //halSpiWriteReg(CCxxx0_RCCTRL1, RCC1);
  //halSpiWriteReg(CCxxx0_RCCTRL0, RCC0);
 
  //��SO�� ���ó�֪ͨ�� ���õ�
  //halSpiWriteReg(CCxxx0_IOCFG0, 0x06);  //0x24);
  //halSpiWriteReg(CCxxx0_IOCFG2, 0x06);  //0x24);
  
  halSpiStrobe(CCxxx0_SFRX); 
 
  halSpiStrobe(CCxxx0_SWORRST);      //��λ�� �¼�1
  halSpiStrobe(CCxxx0_SWOR);         //����WOR	
  return 1;
}

//*****************************************************************************************
//��������void Rf_wakeup() interrupt	2 
//���룺��
//�������
//�������������Ѻ󣬹ر�PD����λwor_flag
//***************************************************************************************** 
void Rf_wakeup() interrupt	2
{
	 INT1_OFF;
	 PCON &= PD_OFF; 
	 LED_D1 = ~LED_D1; 
	 
	 CC1101_Worwakeup();
	 //g_wor_flag = 0x55;
	 
	 //Log_printf("Enter wor\n");
	 //INT1_ON;												//2013��8��15��16:22:59
}

//*****************************************************************************************
//��������INT8U CC1101_InitWOR(INT32U Time) 
//���룺��
//�������
//�������������Ѻ󣬹ر�PD����λwor_flag
//***************************************************************************************** 
//INT8U CC1101_InitWOR(INT32U Time)
//{
//  //INT16U T_Event0=60;   //�� EVENT0��ʱ���趨Ϊ1S
//  INT32U EVENT0=0;
//  INT16U WOR_RES=1;
//  INT16U WOR_rest=1;      //2^(5*WOR_RES) ��ֵ
// 
//  //WORmode =1; //����WORMODģʽ
// 
//  //���������� �����Ϲ����ʱ�򷵻ش���
//  if(Time<15 | Time>61946643) return 0;
// 
//  /* WOR WOR_RES�趨
//  ��WOR_RES�������ֵ����ʱ�� ����WOR_RES��С
// 
//  WOR_RESֵ       ʱ��(�������ֵ)(ms)
//  0                1890.4615         *14.34 (��Сֵ)
//  1                60494.7692
//  2                1935832.6153
//  3                61946643.6923
//  */
//  if(Time<1890) WOR_RES=0;
//  else if(Time<60494)       WOR_RES=1;
//  else if(Time<1935832)     WOR_RES=2;
//  else if(Time<61946643)    WOR_RES=3;
// 
//  // WOR_rest Ĭ�ϵ���1
//  // WOR_rest=2^5WOR_RES
//  /*
//  if(!WOR_RES) WOR_rest=1;
//  else{
//  for(INT8U t=0;t<(5*WOR_RES);t++)WOR_rest *= 2;
//}
//  */
//  //2�Ķ��ٴη������ƶ���λ
//  WOR_rest <<= 5*WOR_RES;
// 
//  // ���� Event0 timeout  (RX ��ѯ���ʱ��);
//  // �¼�0 EVENT0ʱ�䳤�ȹ�ʽ T_event0 = 750 / f_xosc * EVENT0 * 2^(5*WOR_RES) = 1 s,   f_xosc ʹ�õ��� 26 MHz
//  // EVENT0 = (F_xosc*Time)/((750*WOR_rest)*Tms);
// 
//  //���ڼ����ֵ�ձ�ƫ��,����ճ������������, ���Էֶδ���
//  EVENT0 = F_xosc/1000;
//  if(EVENT0>Time)
//  {
//    EVENT0 = EVENT0*Time;
//    EVENT0 = EVENT0/(750*WOR_rest); 
//  }
//  else
//  {
//    EVENT0 = (Time/(750*WOR_rest))*EVENT0;
//  }
// 
//  halSpiStrobe(CCxxx0_SIDLE); //����ģʽ
//  // ���ý��ճ�ʱ Rx_timeout =2.596 ms.
//  // MCSM2.RX_TIME = 001b
//  // => Rx_timeout = EVENT0*C(RX_TIME, WOR_RES)
//  halSpiWriteReg(CCxxx0_MCSM2, 0x10);  //RX_TIME 0   ռ�ձ����
//  // Enable automatic FS calibration when going from IDLE to RX/TX/FSTXON (in between EVENT0 and EVENT1)
//  //��TX,RX�� �Զ�У׼   XSOCʱ�� (10) 149-155uS
//  halSpiWriteReg(CCxxx0_MCSM0, 0x18);                  //У׼ FS_AUTOCAL[1:0]  01    ��IDLEת��TX OR RXģʽʱ
//  //
//  //д�� �¼�0 ʱ��
//  halSpiWriteReg(CCxxx0_WOREVT1, (INT8U)(EVENT0>>8));        // High byte Event0 timeout
//  halSpiWriteReg(CCxxx0_WOREVT0, (INT8U)EVENT0);             // Low byte Event0 timeout.
// 
//  // ���� WOR RCosc У׼
//  // ��Ϊ�������ߺ�ֻʹ��RCƵ������,RC�ܻ������¶�Ӱ��ϴ�,���Ա���һ��ʱ�����WOR���Ѻ�����У׼һ��ʱ��.
//  // ��WORû����֮ǰ RC�����������
//  // tEvent1 ʱ������Ϊ���,���� T_event1 ~ 1.4 ms
//  halSpiWriteReg(CCxxx0_WORCTRL, 0x78| WOR_RES);             //tEvent1 =0111
//  //--RC_CAL =1 �Զ�У׼
//  //halWait(30);                                                //�ȴ�У׼���
//  //halSpiWriteReg(CCxxx0_WORCTRL, 0x70 | WOR_RES);           // tEvent1 =0111 �� 48 (1.333-1.385 ms)
//  // RC_CAL =0
// 
//  //halSpiWriteReg(CCxxx0_RCCTRL1, RCC1);
//  //halSpiWriteReg(CCxxx0_RCCTRL0, RCC0);
// 
//  //��SO�� ���ó�֪ͨ�� �������ݹ���ʱ �õ�
//  halSpiWriteReg(CCxxx0_IOCFG0, 0x06);  //0x24);
//  //halSpiWriteReg(CCxxx0_IOCFG2, 0x06);  //0x24);
//  
//  halSpiStrobe(CCxxx0_SFRX); 
// 
//  halSpiStrobe(CCxxx0_SWORRST);      //��λ�� �¼�1
//  halSpiStrobe(CCxxx0_SWOR);         //����WOR
// 
//  //  halSpiStrobe(CCxxx0_SPWD); //����ϵ�ģʽ
//  return 1;
//}

//*****************************************************************************************
//��������void CC1100_wake up_carry(INT8U ms) 
//���룺INT8U ms �趨���Ѳ�ʱ��
//�������
//�������������ͻ��Ѳ�
//***************************************************************************************** 
// 300ms 313��
// 1s		 x=1044  
// 
void CC1101_Wakeupcarry(INT8U *worcarry, INT8U size,INT8U s)
{
		INT16U	s_count;
		//s_count = s*1044/5;
		s_count = s*48;

		while(s_count--)
		{
			halRfSendPacket(worcarry, size);
			//i = halSpiReadStatus(CCxxx0_TXBYTES);
			//Usart_printf(&s_count,1);
		}

}

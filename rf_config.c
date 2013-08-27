// Filename:	mcu_config.c
// Function:	rf底层驱动
// Author:		wzd
// Date:			2013年8月15日10:21:25


#include "rf_config.h"

/*****************************************************************************************
//函数名：void halWait(INT16U timeout) 
//输入：无
//输出：无
//功能描述：rf操作中的延时函数
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
//函数名：void RESET_CC1100(void)
//输入：无
//输出：无
//功能描述：复位CC1100
//*****************************************************************************************
void RESET_CC1100(void) 
{
    CSN = 0; 
    while (MISO);
    SpiTxRxByte(CCxxx0_SRES); 		//写入复位命令
    while (MISO); 
    CSN = 1; 
}

//*****************************************************************************************
//函数名：void POWER_UP_RESET_CC1100(void) 
//输入：无
//输出：无
//功能描述：上电复位CC1100
//*****************************************************************************************
void POWER_UP_RESET_CC1100(void) 
{
    CSN = 1; 
    halWait(10); 
    CSN = 0; 
    halWait(10); 
    CSN = 1; 
    halWait(410); 
    RESET_CC1100();   		//复位CC1100
}

//*****************************************************************************************
//函数名：void halSpiWriteReg(INT8U addr, INT8U value)
//输入：地址和配置字
//输出：无
//功能描述：SPI写寄存器
//*****************************************************************************************
void halSpiWriteReg(INT8U addr, INT8U value) 
{
    CSN = 0;
    while (MISO);
    SpiTxRxByte(addr);		//写地址
    SpiTxRxByte(value);		//写入配置
    CSN = 1;
}

//*****************************************************************************************
//函数名：void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//输入：地址，写入缓冲区，写入个数
//输出：无
//功能描述：SPI连续写配置寄存器
//*****************************************************************************************
void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
{
    INT8U i, temp;
    temp = addr | WRITE_BURST;			//置位突发位，进行突发存取
    CSN = 0;												//开始突发存取
    while (MISO);
    SpiTxRxByte(temp);
    for (i = 0; i < count; i++)
    {
        SpiTxRxByte(buffer[i]);
    }
    CSN = 1;												//通过置高CSN来终止突发存取
}

//*****************************************************************************************
//函数名：void halSpiStrobe(INT8U strobe)
//输入：命令
//输出：无
//功能描述：SPI写命令
//*****************************************************************************************
void halSpiStrobe(INT8U strobe) 
{
    CSN = 0;
    while (MISO);
    SpiTxRxByte(strobe);		//写入命令
    CSN = 1;
}

//*****************************************************************************************
//函数名：INT8U halSpiReadReg(INT8U addr)
//输入：地址
//输出：该寄存器的配置字
//功能描述：SPI读寄存器
//*****************************************************************************************
INT8U halSpiReadReg(INT8U addr) 
{
    INT8U temp, value;
    temp = addr|READ_SINGLE;//读寄存器命令
    CSN = 0;
    while (MISO);
    SpiTxRxByte(temp);
    value = SpiTxRxByte(0);
    CSN = 1;
    return value;
}

//*****************************************************************************************
//函数名：void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//输入：地址，读出数据后暂存的缓冲区，读出配置个数
//输出：无
//功能描述：SPI连续写配置寄存器
//*****************************************************************************************
void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
{
    INT8U i,temp;
    temp = addr | READ_BURST;		//写入要读的配置寄存器地址和读命令
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
//函数名：INT8U halSpiReadReg(INT8U addr)
//输入：地址
//输出：该状态寄存器当前值
//功能描述：SPI读状态寄存器
//*****************************************************************************************
INT8U halSpiReadStatus(INT8U addr) 
{
    INT8U value,temp;
    temp = addr | READ_BURST;		//写入要读的状态寄存器的地址同时写入读命令
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
//    temp = addr | READ_SINGLE;		//写入要读的状态寄存器的地址同时写入读命令
//    CSN = 0;
//    while (MISO);
//    SpiTxRxByte(temp);
//    value = SpiTxRxByte(0);
//    CSN = 1;
//    return value;
//}

//*****************************************************************************************
//函数名：void halRfWriteRfSettings(RF_SETTINGS *pRfSettings)
//输入：无
//输出：无
//功能描述：配置CC1100的寄存器
//*****************************************************************************************
void halRfWriteRfSettings(void) 
{
    halSpiWriteReg(CCxxx0_IOCFG2,   rfSettings.IOCFG2);
    halSpiWriteReg(CCxxx0_IOCFG0,   rfSettings.IOCFG0); 
    
    halSpiWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL2);//自已加的
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
//函数名：void halRfSendPacket(INT8U *txBuffer, INT8U size)
//输入：发送的缓冲区，发送数据个数
//输出：无
//功能描述：CC1101发送一组数据
//*****************************************************************************************
void halRfSendPacket(INT8U *txBuffer, INT8U size) 
{
		// 配置了IOCFG0.GDO0_CFG=0x06 发送/接收到同步字时置位，并在数据包的末尾取消置位
		//INT1_OFF;
		// 首次初始化，开启可变数据包长度，长度字节被首先写入，没有开启地址识别，第二个字节无需写入ADR
    halSpiWriteReg(CCxxx0_TXFIFO, size);
    halSpiWriteBurstReg(CCxxx0_TXFIFO, txBuffer, size);	//写入要发送的数据
    
    halSpiStrobe(CCxxx0_STX);		//进入发送模式发送数据
    
    // Wait for GDO0 to be set -> sync transmitted
    while (!GDO0);
    // Wait for GDO0 to be cleared -> end of packet 而后切换到IDLE模式
    while (GDO0);
    // 发送指令，清除TX_FIFO
    halSpiStrobe(CCxxx0_SFTX);
    //INT1_ON;
}

//*****************************************************************************************
//函数名：INT8U halRfReceivePacket(INT8U *rxBuffer, INT8U *length) 
//输入：INT8U *rxBuffer 无线接收数据，INT8U *length 接收字节数
//输出：返回接收到的字节数
//功能描述：CC1101将接收到的无线数据存储到*rxBuffer指向的数组中
//*****************************************************************************************
INT8U halRfReceivePacket(INT8U *rxBuffer, INT8U *length) 
{
    INT8U status[2];
    INT8U packetLength;
    INT8U i=(*length)*4;  // 具体多少要根据datarate和length来决定
    
    //Log_printf("Enter rx  ");
    halSpiStrobe(CCxxx0_SRX);		//进入接收状态
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
    
    // CCxxx0_RXBYTES RX_FIFO的字节数
    if ((halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) //如果接的字节数不为0
    {
        packetLength = halSpiReadReg(CCxxx0_RXFIFO);//读出第一个字节，此字节为该帧数据长度
	
				//测试程序
				//Usart_printf(&packetLength,1);

        if (packetLength <= *length) 		//如果所要的有效数据长度小于等于接收到的数据包的长度
        {
        	//Log_printf("rx data  ");
            halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength); //读出所有接收到的数据
            *length = packetLength;				//把接收数据长度的修改为当前数据的长度
            
            Usart_printf(rxBuffer,packetLength);

            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 	//读出CRC校验位
            halSpiStrobe(CCxxx0_SFRX);		//清洗接收缓冲区
            return (status[1] & CRC_OK);			//如果校验成功返回接收成功
        }
        else 
        {
        	//Log_printf("length big  ");
            *length = packetLength;
            halSpiStrobe(CCxxx0_SFRX);		//清洗接收缓冲区
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
//函数名：INT8U CC1101_EnterRx(INT8U *rxBuffer) 
//输入：INT8U *rxBuffer 无线接收数据
//输出：返回接收到的字节数
//功能描述：进入全速处理模式，提取数据帧
//*****************************************************************************************
//INT8U CC1101_EnterRx(INT8U *rxBuffer) 
//{
//    INT8U status[2],wor_data[2];
//    INT8U packetLength=0;
//
//    halSpiStrobe(CCxxx0_SRX);		//进入接收状态 
//    if ((halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) //如果接的字节数不为0
//    {
//        packetLength = halSpiReadReg(CCxxx0_RXFIFO);//读出第一个字节，此字节为该帧数据长度
//				//delay(100);
//				Usart_printf(&packetLength,1);
//		
//				if ( packetLength > 2 )	
//		    {		
//		        halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength); //读出所有接收到的数据
//		        //*length = packetLength;				//把接收数据长度的修改为当前数据的长度
//		        //Usart_printf(rxBuffer,packetLength);
//		        // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
//		        halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 	//读出CRC校验位
//		        halSpiStrobe(CCxxx0_SFRX);		//清洗接收缓冲区
//		        //return (status[1] & CRC_OK);			//如果校验成功返回接收成功
//		        if( status[1] & CRC_OK )
//		        {
//		        	Usart_printf(rxBuffer,packetLength);
//		        	// ？？？？？？？？？？？？？？此处增加接收数据校验，判断是否为本条数据的帧，而后，送予路由逻辑进行处理
//		        	// ？？？？？？？？？？？？？？
//		        	return packetLength;
//		        }
//		        else
//		        {	
//		        	return 0;
//						}
//		    }
//
//        if ( packetLength == 2 ) 																		//如果所要的有效数据长度等于接收到的数据包的长度
//        {
//            halSpiReadBurstReg(CCxxx0_RXFIFO, wor_data, packetLength); 	//读出所有接收到的数据
//            
//						if ( (wor_data[0]==BROADCAST && wor_data[1]==BROADCAST) || ( (wor_data[0]==g_module_id) && (wor_data[1]==g_module_id>>8) ) )
//						{
//	            //Log_printf("Enter Rx\n");
//							Usart_printf(wor_data,2);
//	            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
//	            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 						//读出CRC校验位
//	            halSpiStrobe(CCxxx0_SFRX);																//清洗接收缓冲区
//	            if(status[1] & CRC_OK)
//	            {
//								halSpiStrobe(CCxxx0_SFRX); 
//								g_rf_rx_flag = 0x55;																	// 标志置位后，进入全速接收模式
//								Timer0_Init(1);
//								TIMER0_ON;																						// 开启定时器0 定时1s																				// 开启定时器0 定时1s
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
//        halSpiStrobe(CCxxx0_SFRX);		//清洗接收缓冲区
//        return 0;
//    } 
//    else
//        return 0;
//}

INT8U halRfRxPacket(INT8U *rxBuffer) 
{
    INT8U status[2],wor_data[2];
    INT8U packetLength=0;
    //INT8U i=(*length)*4;  																					// 具体多少要根据datarate和length来决定

    halSpiStrobe(CCxxx0_SRX);																					// 进入接收状态

    if ((halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) 				// 如果接的字节数不为0
    {
        packetLength = halSpiReadReg(CCxxx0_RXFIFO);									// 读出第一个字节，此字节为该帧数据长度
				delay(100);
				//Usart_printf(&packetLength,1);
		
		if ( packetLength > 2 )	
	    {		
	        halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength); 	// 读出所有接收到的数据
	        //*length = packetLength;																		// 把接收数据长度的修改为当前数据的长度
	        //Usart_printf(rxBuffer,packetLength);
	        // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
	        halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 							// 读出CRC校验位
	        halSpiStrobe(CCxxx0_SFRX);																	// 清洗接收缓冲区
	        //return (status[1] & CRC_OK);															// 如果校验成功返回接收成功
	        if( status[1] & CRC_OK )
	        {
						//TIMER0_OFF;																								// 关闭定时器
	        	//g_wor_flag = 0x00;																				// 接收到数据后，退出全速接收模式
	        	Usart_printf(rxBuffer,packetLength);
	        	halRfSendPacket(Test, 13);																// 应答信息
	        	
						LED_D3 = ~LED_D3;
	        	return packetLength;
	        }
	        else
	        {	
	        	Log_printf("3 and crc\n");
	        	return 0;
					}
	    }

      if (packetLength == 2) 																					// 如果所要的有效数据长度等于接收到的数据包的长度
      {
          halSpiReadBurstReg(CCxxx0_RXFIFO, wor_data, packetLength); 	// 读出所有接收到的数据
					if ( (wor_data[0]==BROADCAST && wor_data[1]==BROADCAST) || ( (wor_data[0]==g_module_id.Sn[0]) && (wor_data[1]==g_module_id.Sn[1]) ) )
					//if ( (wor_data[0]==0x55 && wor_data[1]==0xAA) || (wor_data[0]==g_module_id && wor_data[1]==g_module_id>>8) )
					{
            //Log_printf("Enter wor\n");
            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 						// 读出CRC校验位
            halSpiStrobe(CCxxx0_SFRX);																// 清洗接收缓冲区
            if(status[1] & CRC_OK)
            {
							halSpiStrobe(CCxxx0_SFRX);
							timer = 0; 
							Timer0_Init(1);
							TIMER0_ON;																							// 接收到唤醒波，就刷新定时时间，定时1s
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
        halSpiStrobe(CCxxx0_SFRX);		//清洗接收缓冲区
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
//    halSpiStrobe(CCxxx0_SRX);		//进入接收状态 
//    if ((halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) //如果接的字节数不为0
//    {
//        packetLength = halSpiReadReg(CCxxx0_RXFIFO);//读出第一个字节，此字节为该帧数据长度
//				//delay(100);
//				Usart_printf(&packetLength,1);
//		
//				if ( packetLength > 2 )	
//		    {		
//		        halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength); //读出所有接收到的数据
//		        //*length = packetLength;				//把接收数据长度的修改为当前数据的长度
//		        //Usart_printf(rxBuffer,packetLength);
//		        // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
//		        halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 	//读出CRC校验位
//		        halSpiStrobe(CCxxx0_SFRX);		//清洗接收缓冲区
//		        //return (status[1] & CRC_OK);			//如果校验成功返回接收成功
//		        if( status[1] & CRC_OK )
//		        {
//		        	Usart_printf(rxBuffer,packetLength);
//		        	g_rf_rx_flag = 0x00;
//		        	// ？？？？？？？？？？？？？？此处增加接收数据校验，判断是否为本条数据的帧，而后，送予路由逻辑进行处理
//		        	// ？？？？？？？？？？？？？？
//		        	return packetLength;
//		        }
//		        else
//		        {	
//		        	return 0;
//						}
//		    }
//
//        if ( packetLength == 2 ) 																		//如果所要的有效数据长度等于接收到的数据包的长度
//        {
//            halSpiReadBurstReg(CCxxx0_RXFIFO, wor_data, packetLength); 	//读出所有接收到的数据
//            
//						if ( (wor_data[0]==BROADCAST && wor_data[1]==BROADCAST) || ( (wor_data[0]==g_module_id) && (wor_data[1]==g_module_id>>8) ) )
//						{
//	            //Log_printf("Enter Rx\n");
//							Usart_printf(wor_data,2);
//	            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
//	            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 						//读出CRC校验位
//	            halSpiStrobe(CCxxx0_SFRX);																//清洗接收缓冲区
//	            if(status[1] & CRC_OK)
//	            {
//								halSpiStrobe(CCxxx0_SFRX); 
//								g_rf_rx_flag = 0x55;																	// 标志置位后，进入全速接收模式
//								//Timer0_Init(1);
//								//TIMER0_ON;																						// 开启定时器0 定时1s																				// 开启定时器0 定时1s
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
//        halSpiStrobe(CCxxx0_SFRX);		//清洗接收缓冲区
//        return 0;
//    } 
//    else
//        return 0;
//}

//*****************************************************************************************
//函数名：INT8U CC1101_Worwakeup(INT8U *rxBuffer, INT8U *length)  
//输入：INT8U *rxBuffer 无线接收数据，INT8U *length 接收字节数
//输出：返回接收到的字节数
//功能描述：CC1101将接收到的无线数据存储到*rxBuffer指向的数组中
//*****************************************************************************************
//INT8U CC1101_Worwakeup(void) 
//{
//    INT8U wor_data[2],status[2];
//    INT8U packetLength;
//
//    // 检测到数据包后，进入接收模式
//    halSpiStrobe(CCxxx0_SRX);
//    // CCxxx0_RXBYTES RX_FIFO的字节数
//    if ( (halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO) ) 						//如果接的字节数不为0
//    {
//        	packetLength = halSpiReadReg(CCxxx0_RXFIFO);										//读出第一个字节，此字节为该帧数据长度
//					//Usart_printf(&packetLength,1);
//				
//	        if (packetLength == 2) 																		//如果所要的有效数据长度等于接收到的数据包的长度
//	        {
//	            halSpiReadBurstReg(CCxxx0_RXFIFO, wor_data, packetLength); 	//读出所有接收到的数据
//	            //*length = packetLength;																		//把接收数据长度的修改为当前数据的长度
//							if ( (wor_data[0]==BROADCAST && wor_data[1]==BROADCAST) || ( (wor_data[0]==g_module_id) && (wor_data[1]==g_module_id>>8) ) )
//							//if ( (wor_data[0]==0x55 && wor_data[1]==0xAA) || (wor_data[0]==g_module_id && wor_data[1]==g_module_id>>8) )
//							{
//		            //Log_printf("Enter wor\n");
//		            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
//		            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 						//读出CRC校验位
//		            halSpiStrobe(CCxxx0_SFRX);																//清洗接收缓冲区
//		            if(status[1] & CRC_OK)
//		            {
//		            	
//									halSpiStrobe(CCxxx0_SFRX); 
//									g_rf_rx_flag = 0x55;																	// 标志置位后，进入全速接收模式
//									//g_rx_timeout = 0x00;
//									Timer0_Init(1);
//									TIMER0_ON;																						// 开启定时器0 定时1s
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
//		halSpiStrobe(CCxxx0_SWORRST);      																	//复位到 事件1
//		halSpiStrobe(CCxxx0_SWOR);         																	//启动WOR	
//		INT1_ON;												//2013年8月15日16:22:59
//    return 0;
//}

INT8U CC1101_Worwakeup(void) 
{
    INT8U wor_data[2],status[2];
    INT8U packetLength;

    // 检测到数据包后，进入接收模式
    halSpiStrobe(CCxxx0_SRX);
    // CCxxx0_RXBYTES RX_FIFO的字节数
    if ( (halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO) ) 						//如果接的字节数不为0
    {
        	packetLength = halSpiReadReg(CCxxx0_RXFIFO);										//读出第一个字节，此字节为该帧数据长度
					//Usart_printf(&packetLength,1);
				
	        if (packetLength == 2) 																		//如果所要的有效数据长度等于接收到的数据包的长度
	        {
	            halSpiReadBurstReg(CCxxx0_RXFIFO, wor_data, packetLength); 	//读出所有接收到的数据
	            //*length = packetLength;																		//把接收数据长度的修改为当前数据的长度
							if ( (wor_data[0]==BROADCAST && wor_data[1]==BROADCAST) || ( (wor_data[0]==g_module_id.Sn[0]) && (wor_data[1]==g_module_id.Sn[1]) ) )
							//if ( (wor_data[0]==0x55 && wor_data[1]==0xAA) || (wor_data[0]==g_module_id && wor_data[1]==g_module_id>>8) )
							{
		            //Log_printf("Enter wor\n");
		            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
		            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 						//读出CRC校验位
		            halSpiStrobe(CCxxx0_SFRX);																//清洗接收缓冲区
		            if(status[1] & CRC_OK)
		            {
		            	
									halSpiStrobe(CCxxx0_SFRX); 
									g_wor_flag = 0x55;																		// Wor唤醒置位，之后进入全速接收模式
									
									timer = 0;
									Timer0_Init(1);
									TIMER0_ON;																						// 开启定时器0 定时1s
		            	
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
		halSpiStrobe(CCxxx0_SWORRST);      																	//复位到 事件1
		halSpiStrobe(CCxxx0_SWOR);         																	//启动WOR	
		INT1_ON;												//2013年8月15日16:22:59
    return 0;
}

//*****************************************************************************************
//函数名：INT8U CC1101_Setwor(void)  
//输入：无
//输出：无
//功能描述：设置rf唤醒模式参数 EVENT0=300ms  rx_timeout=37.5ms
//***************************************************************************************** 
INT8U CC1101_Setwor(void)
{
 	halSpiStrobe(CCxxx0_SIDLE); //空闲模式
  // MCSM2.RX_TIME = 001b
  //	Rx_timeout = （10400*3.6058）us = 37.5ms
  // => Rx_timeout = (EVENT0*C(RX_TIME, WOR_RES))*26/x   x晶振频率
  halSpiWriteReg(CCxxx0_MCSM2, 0x00); 			//主要计算wor中Rx的接收时限，Rx的duty cycle
  // Enable automatic FS calibration when going from IDLE to RX/TX/FSTXON (in between EVENT0 and EVENT1)
  //在TX,RX后 自动校准   XSOC时限 (10) 149-155uS
  //MSCM1 默认值 RXOFF_MODE 和 TXOFF_MODE 接收到数据包后 转换到IDLE模式
  halSpiWriteReg(CCxxx0_MCSM0, 0x18);                  //校准 FS_AUTOCAL[1:0]  01    从IDLE转到TX OR RX模式时
  
	//配置EVENT0时间为300ms，Tx一帧唤醒波12个字节(960us)
  halSpiWriteReg(CCxxx0_WOREVT1, 0x28);        			// High byte Event0 timeout
  halSpiWriteReg(CCxxx0_WOREVT0, 0xA0);             // Low byte Event0 timeout.
 
  // 启动 WOR RCosc 校准
  // 因为进入休眠后只使用RC频率周期,RC受环境和温度影响较大,所以必须一段时间或者WOR唤醒后重新校准一次时钟.
  // 在WOR没启动之前 RC须得先行启动
  // tEvent1 时间设置为最大,设置 T_event1 ~ 1.4 ms
  halSpiWriteReg(CCxxx0_WORCTRL, 0x78);             		//tEvent1 =0111 接收的最大时限 
  //--RC_CAL =1 自动校准
  //halWait(30);                                                //等待校准完成
  //halSpiWriteReg(CCxxx0_WORCTRL, 0x70 | WOR_RES);           // tEvent1 =0111 即 48 (1.333-1.385 ms)
  // RC_CAL =0
 
  //halSpiWriteReg(CCxxx0_RCCTRL1, RCC1);
  //halSpiWriteReg(CCxxx0_RCCTRL0, RCC0);
 
  //把SO口 设置成通知口 当置低
  //halSpiWriteReg(CCxxx0_IOCFG0, 0x06);  //0x24);
  //halSpiWriteReg(CCxxx0_IOCFG2, 0x06);  //0x24);
  
  halSpiStrobe(CCxxx0_SFRX); 
 
  halSpiStrobe(CCxxx0_SWORRST);      //复位到 事件1
  halSpiStrobe(CCxxx0_SWOR);         //启动WOR	
  return 1;
}

//*****************************************************************************************
//函数名：void Rf_wakeup() interrupt	2 
//输入：无
//输出：无
//功能描述：唤醒后，关闭PD，置位wor_flag
//***************************************************************************************** 
void Rf_wakeup() interrupt	2
{
	 INT1_OFF;
	 PCON &= PD_OFF; 
	 LED_D1 = ~LED_D1; 
	 
	 CC1101_Worwakeup();
	 //g_wor_flag = 0x55;
	 
	 //Log_printf("Enter wor\n");
	 //INT1_ON;												//2013年8月15日16:22:59
}

//*****************************************************************************************
//函数名：INT8U CC1101_InitWOR(INT32U Time) 
//输入：无
//输出：无
//功能描述：唤醒后，关闭PD，置位wor_flag
//***************************************************************************************** 
//INT8U CC1101_InitWOR(INT32U Time)
//{
//  //INT16U T_Event0=60;   //把 EVENT0的时间设定为1S
//  INT32U EVENT0=0;
//  INT16U WOR_RES=1;
//  INT16U WOR_rest=1;      //2^(5*WOR_RES) 的值
// 
//  //WORmode =1; //开启WORMOD模式
// 
//  //当输入数据 不符合规则的时候返回错误
//  if(Time<15 | Time>61946643) return 0;
// 
//  /* WOR WOR_RES设定
//  以WOR_RES所能区分的最大时限 区分WOR_RES大小
// 
//  WOR_RES值       时间(极限最大值)(ms)
//  0                1890.4615         *14.34 (最小值)
//  1                60494.7692
//  2                1935832.6153
//  3                61946643.6923
//  */
//  if(Time<1890) WOR_RES=0;
//  else if(Time<60494)       WOR_RES=1;
//  else if(Time<1935832)     WOR_RES=2;
//  else if(Time<61946643)    WOR_RES=3;
// 
//  // WOR_rest 默认等于1
//  // WOR_rest=2^5WOR_RES
//  /*
//  if(!WOR_RES) WOR_rest=1;
//  else{
//  for(INT8U t=0;t<(5*WOR_RES);t++)WOR_rest *= 2;
//}
//  */
//  //2的多少次方，右移多少位
//  WOR_rest <<= 5*WOR_RES;
// 
//  // 设置 Event0 timeout  (RX 轮询间隔时间);
//  // 事件0 EVENT0时间长度公式 T_event0 = 750 / f_xosc * EVENT0 * 2^(5*WOR_RES) = 1 s,   f_xosc 使用的是 26 MHz
//  // EVENT0 = (F_xosc*Time)/((750*WOR_rest)*Tms);
// 
//  //由于计算的值普遍偏大,如果照常计算会出现溢出, 所以分段处理
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
//  halSpiStrobe(CCxxx0_SIDLE); //空闲模式
//  // 设置接收超时 Rx_timeout =2.596 ms.
//  // MCSM2.RX_TIME = 001b
//  // => Rx_timeout = EVENT0*C(RX_TIME, WOR_RES)
//  halSpiWriteReg(CCxxx0_MCSM2, 0x10);  //RX_TIME 0   占空比最大
//  // Enable automatic FS calibration when going from IDLE to RX/TX/FSTXON (in between EVENT0 and EVENT1)
//  //在TX,RX后 自动校准   XSOC时限 (10) 149-155uS
//  halSpiWriteReg(CCxxx0_MCSM0, 0x18);                  //校准 FS_AUTOCAL[1:0]  01    重IDLE转到TX OR RX模式时
//  //
//  //写入 事件0 时间
//  halSpiWriteReg(CCxxx0_WOREVT1, (INT8U)(EVENT0>>8));        // High byte Event0 timeout
//  halSpiWriteReg(CCxxx0_WOREVT0, (INT8U)EVENT0);             // Low byte Event0 timeout.
// 
//  // 启动 WOR RCosc 校准
//  // 因为进入休眠后只使用RC频率周期,RC受环境和温度影响较大,所以必须一段时间或者WOR唤醒后重新校准一次时钟.
//  // 在WOR没启动之前 RC须得先行启动
//  // tEvent1 时间设置为最大,设置 T_event1 ~ 1.4 ms
//  halSpiWriteReg(CCxxx0_WORCTRL, 0x78| WOR_RES);             //tEvent1 =0111
//  //--RC_CAL =1 自动校准
//  //halWait(30);                                                //等待校准完成
//  //halSpiWriteReg(CCxxx0_WORCTRL, 0x70 | WOR_RES);           // tEvent1 =0111 即 48 (1.333-1.385 ms)
//  // RC_CAL =0
// 
//  //halSpiWriteReg(CCxxx0_RCCTRL1, RCC1);
//  //halSpiWriteReg(CCxxx0_RCCTRL0, RCC0);
// 
//  //把SO口 设置成通知口 当有数据过来时 置低
//  halSpiWriteReg(CCxxx0_IOCFG0, 0x06);  //0x24);
//  //halSpiWriteReg(CCxxx0_IOCFG2, 0x06);  //0x24);
//  
//  halSpiStrobe(CCxxx0_SFRX); 
// 
//  halSpiStrobe(CCxxx0_SWORRST);      //复位到 事件1
//  halSpiStrobe(CCxxx0_SWOR);         //启动WOR
// 
//  //  halSpiStrobe(CCxxx0_SPWD); //进入断电模式
//  return 1;
//}

//*****************************************************************************************
//函数名：void CC1100_wake up_carry(INT8U ms) 
//输入：INT8U ms 设定唤醒波时常
//输出：无
//功能描述：发送唤醒波
//***************************************************************************************** 
// 300ms 313个
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

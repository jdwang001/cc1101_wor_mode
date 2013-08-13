#ifndef _MCU_CONFIG_H__
#define _MCU_CONFIG_H__

#include "stc_new_8051.h"
#include <intrins.h>
#define		INT8U	unsigned char
#define		INT16U	unsigned int
#define		INT32U	unsigned long
#define		PD_ON	0x02
#define		PD_OFF	0xFD

sbit    MOSI    =   P0^0;   
sbit    SCK     =   P0^1;   
sbit    MISO    =   P0^2;
sbit    GDO2    =   P0^3;   
//sbit    GDO0    =   P0^4;                               //0
sbit    CSN     =   P3^3;

// �˴���Ҫ��GDO0�ܽ����ӵ��ⲿ�ж��ϣ�������Ϊ�͵�ƽ�жϡ�
sbit	GDO0	=	P3^2;		// INT0

//sbit    LED_R   =   P1^7;

sbit LED_R  =   P3^7;
sbit LED_B0 =   P2^0;

extern INT8U _1s_counter,leng,count; 
extern INT8U flag_rx;

extern INT8U TxBuf[64];	 			// 11�ֽ�, �����Ҫ���������ݰ�,����ȷ����
extern INT8U RxBuf[64];

void SpiInit(void);
void CpuInit(void);
INT8U SpiTxRxByte(INT8U dat);
void UART_init();
void Log_printf(INT8U *p_log);
void Usart_printf(INT8U *p_uart,INT8U num);
void delay(unsigned int s);
void ser();

#endif
#ifndef _MCU_CONFIG_H__
#define _MCU_CONFIG_H__

#include <intrins.h>
#include "stc_new_8051.h"

#define		INT8U			unsigned char
#define		INT16U		unsigned int
#define		INT32U		unsigned long
#define		PD_ON			0x02
#define		PD_OFF		0xFD
#define		G_IT_OFF	EA=0
#define		G_IT_ON		EA=1						// 定义打开全局中断宏
#define		INT1_OFF	EX1=0
#define		INT1_ON 	EX1=1						// 定义打开INT1中断宏

sbit	MOSI	=	P0^0;   
sbit	SCK		=	P0^1;   
sbit	MISO	=	P0^2;
sbit	GDO2	=	P0^3;   
sbit	GDO0	=	P3^3;						// INT1外部中断INT1 低电平触发
sbit	CSN		=	P0^4;

// 此处需要将GDO0管脚连接到外部中断上，并配置为低电平中断。
//sbit	GDO0	=	P3^2;		// INT0
//sbit    LED_R   =   P1^7;

sbit LED_R 	=	P3^7;
sbit LED_D1 = P2^0;
sbit LED_D2	=	P2^1;
sbit LED_D3 = P2^2;
sbit LED_D4	=	P2^3;

extern INT16U	g_module_id;
extern INT8U g_1s_counter,g_leng,g_count,g_test_count; 
extern INT8U g_rx_flag;
extern INT8U TxBuf[64];	 			// 11字节, 如果需要更长的数据包,请正确设置
extern INT8U RxBuf[64];

void Int1Init(void);
void SpiInit(void);
void CpuInit(void);
void UART_init(void);
void Log_printf(INT8U *p_log);
void Usart_printf(INT8U *p_uart,INT8U num);
void delay(unsigned int s);
void uart_isr();
INT8U SpiTxRxByte(INT8U dat);

#endif

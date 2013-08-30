#ifndef _MCU_CONFIG_H__
#define _MCU_CONFIG_H__

#include <intrins.h>
#include "stc_new_8051.h"


#define		INT8U			unsigned char
#define		INT16U		unsigned int
#define		INT32U		unsigned long
#define		F_mcu			11059200L				// MCU晶振频率 
#define   MCU_ID		0x8F  
#define		PD_ON			0x02
#define		PD_OFF		0xFD
#define		G_IT_OFF	EA=0
#define		G_IT_ON		EA=1						// 定义打开全局中断宏
#define		INT1_OFF	EX1=0
#define		INT1_ON 	EX1=1						// 定义打开INT1中断宏
#define		TIMER0_OFF TR0=0					// 定时器0停止运行
#define		TIMER0_ON  TR0=1					// 定时器0开始运行


/*Define ISP/IAP/EEPROM command*/ 
#define  CMD_IDLE          0                 //Stand-By 
#define  CMD_READ          1                 //Byte-Read 
#define  CMD_PROGRAM       2                 //Byte-Program 
#define  CMD_ERASE         3                 //Sector-Erase 

/*Define ISP/IAP/EEPROM operation const for IAP_CONTR*/ 
//#define  ENABLE_IAP      0x80              //if SYSCLK<30MHz 
//#define  ENABLE_IAP      0x81              //if SYSCLK<24MHz 
#define  ENABLE_IAP        0x82              //if SYSCLK<20MHz 
//#define  ENABLE_IAP      0x83              //if SYSCLK<12MHz 

//Start address for STC12C5A60S2 series EEPROM 
#define   IAP_ADDRESS      	0x0000
#define		SEARCH_MODE			 	0x0000
#define		GATEWAY_ADDRESS	 	0x0200
#define		MODEL_SN_ADDRESS 	0x0400
#define		ROUTEDATA_ADDRESS 0x0500

typedef union MODULE_SN{
 INT8U Sn[2];
 INT16U Sn_temp;
} Module_Sn;

sbit	MOSI	=	P0^0;   
sbit	SCK		=	P0^1;   
sbit	MISO	=	P0^2;
sbit	GDO2	=	P0^3;   
sbit	GDO0	=	P3^3;						              // INT1外部中断INT1 低电平触发
sbit	CSN		=	P0^4;

// 此处需要将GDO0管脚连接到外部中断上，并配置为低电平中断。
//sbit	GDO0	=	P3^2;		// INT0
//sbit    LED_R   =   P1^7;

sbit LED_R 	=	P3^7;
sbit LED_D1 = P2^0;
sbit LED_D2	=	P2^1;
sbit LED_D3 = P2^2;
sbit LED_D4	=	P2^3;

extern INT8U g_wor_flag,g_enter_rx;
extern INT16U timer;
extern Module_Sn g_module_id,g_gateway;
extern INT8U g_1s_counter,g_leng,g_count,g_test_count,g_rf_rx_flag,g_rx_timeout; 
extern INT8U g_rx_flag;
extern INT8U g_search;
extern INT8U TxBuf[64];	 			// 11字节, 如果需要更长的数据包,请正确设置
extern INT8U RxBuf[64];

void IapIdle(); 
INT8U IapReadByte(INT16U addr); 
void IapRead2Array(INT16U addr,INT8U* arraydata,INT8U size);
void IapProgramByte(INT16U addr, INT8U dat); 
void IapEraseSector(INT16U addr); 
INT8U IapCheckEEPROM(INT16U addr,INT8U size);
void IapReadModelSn(INT16U addr,Module_Sn* sndata);
void Int1Init(void);
void SpiInit(void);
void CpuInit(void);
void UART_init(void);
void Log_printf(INT8U *p_log);
void Usart_printf(INT8U *p_uart,INT8U num);
void delay(unsigned int s);
void uart_isr();
INT8U SpiTxRxByte(INT8U dat);
void	Timer0_Init(INT16U ms);
void timer0_isr();

#endif

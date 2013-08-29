// Filename:	rf_route.c
// Function:	rf路由层数据处理
// Author:		wzd
// Date:			2013年8月27日22:43:24


#include "rf_route.h"

// 唤醒分为两种，广播和模块唤醒 此处处理是否是模块唤醒？？？？？？？？？？
// 判断数据是否是发送给本模块
INT8U IsMe(Rf_Route* routepacket)
{
  // 判断 目的地址 和  模块地址 是否相同  剔除路由信息 区分出 模块和基站 模块最高位为0 基站最高位为1
  if( ( g_module_id.Sn_temp&0x8FFF ) == ( routepacket->Des.Sn_temp&0x8FFF ) )
    return 1;
  else
    return 0;
}

//
// 数据结构如下所示：
// Pre Length Rid Key Gateway Route  protocoltype src des data
// 传输数据是，Rid不进行增减，Rid只是针对于路由发现的流程
// 基站进行路由级别分配时Rid = 1开始，模块的routepacket.RfPreRid = 0开始, 网关的判断应该在接收到RF数据后校验和检验正确
void AckARL(Rf_Route* routepacket,INT8U* psentrfdata)
{
	// 先直接截取原结构体数据，返回给基站
	INT8U i;
	INT8U checknum=0;
  INT8U routedatalength=0;
  INT8U routeprotocol=0;
  INT8U datalength=0;

  psentrfdata[0] = routepacket->Pre; 
  psentrfdata[1] = routepacket->Length; 
  psentrfdata[2] = routepacket->Rid;
  psentrfdata[3] = routepacket->Key;
  
  psentrfdata[4] = routepacket->Gateway.Sn[0];
  psentrfdata[5] = routepacket->Gateway.Sn[1];
  
  // 对路由数据进行处理
  routedatalength = (routepacket->RfRouteData.Orien & 0x0F)*2;
  psentrfdata[6] = (routepacket->RfRouteData.Orien&0x0F)|0x50;		// 更改方向
  psentrfdata[7] = routepacket->RfRouteData.CRPL;
  // 当路由深度为0时，pRouteData不指向数组
  if( routedatalength != 0 )
  {
    //routepacket->RfRouteData.pRouteData = &psentrfdata[8];     // 应该将数据放到传入的
    for(i=0;i<routedatalength;i++)
    	psentrfdata[8+i] = *(routepacket->RfRouteData.pRouteData+i);
  }
  // 计算出数据类型后的下标
  routeprotocol = routedatalength +2+6;                              // routelength+2+6 = 下一字段下标
  psentrfdata[routeprotocol] = routepacket->ProtocolType; 
  // 回传数据时，Src和Des互换位置
  psentrfdata[routeprotocol+1] = routepacket->Des.Sn[0];
  psentrfdata[routeprotocol+2] = routepacket->Des.Sn[1];
  psentrfdata[routeprotocol+3] = routepacket->Src.Sn[0];
  psentrfdata[routeprotocol+4] = routepacket->Src.Sn[1];
  // 数据段长度
  datalength = routepacket->Length - ( routedatalength+2 ) - 4 - 5;     // 传感器数据长度
  if( datalength != 0 )
  {
  	for( i=0;i<datalength;i++ )
  		psentrfdata[routeprotocol+5+i] = *(routepacket->pSensorData+i);
  }
  // routepacket->Length长度没有包含 首字节 校验和 长度字节本身
  // 计算校验和
	for( i=0;i<routepacket->Length+3;i++ )  
	{
		psentrfdata[routeprotocol+5+datalength] += psentrfdata[i];
	}
	// 提取出当前路由深度  发送唤醒波定点唤醒下级
	WorCarry[0] = psentrfdata[8+(routepacket->RfRouteData.CRPL-1)*2];
	WorCarry[1] = psentrfdata[8+(routepacket->RfRouteData.CRPL-1)*2+1];
	
	CC1101_Wakeupcarry(WorCarry,2,2);
	halRfSendPacket(psentrfdata,routepacket->Length+3);
	g_rid++;
}

void test(Rf_Route* routepacket,INT8U* psentrfdata)
{
	// 先直接截取原结构体数据，返回给基站
	INT8U i;
	INT8U checknum=0;
  INT8U routedatalength=0;
  INT8U routeprotocol=0;
  INT8U datalength=0;

  psentrfdata[0] = routepacket->Pre; 
  psentrfdata[1] = routepacket->Length; 
  psentrfdata[2] = routepacket->Rid;
  psentrfdata[3] = routepacket->Key;
  
  psentrfdata[4] = routepacket->Gateway.Sn[0];
  psentrfdata[5] = routepacket->Gateway.Sn[1];
  
  if( 0x81 == routepacket->Key )
  	psentrfdata[6] = routepacket->RfRouteData.Orien+1;		// 在搜寻路由传输给基站时自增1
  else
  	psentrfdata[6] = routepacket->RfRouteData.Orien;
  
  // 对路由数据进行处理Orien为0 时  CRPL为1
  routedatalength = (routepacket->RfRouteData.Orien & 0x0F)*2;
  psentrfdata[7] = routepacket->RfRouteData.CRPL;
  // 当路由深度为0时，pRouteData不指向数组
 
  if( routedatalength != 0 )
  {
    //routepacket->RfRouteData.pRouteData = &psentrfdata[8];     // 应该将数据放到传入的
    for(i=0;i<routedatalength;i++)
    	psentrfdata[8+i] = *(routepacket->RfRouteData.pRouteData+i);
  }
  
  
  // 计算出数据类型后的下标
  routeprotocol = routedatalength +2+6;                              // routelength+2+6 = 下一字段下标
  psentrfdata[routeprotocol] = routepacket->ProtocolType; 
  // 数据段长度
  datalength = routepacket->Length - ( routedatalength+2 ) - 4 - 5;     // 传感器数据长度
  if( datalength != 0 )
  {
  	for( i=0;i<datalength;i++ )
  		psentrfdata[routeprotocol+5+i] = *(routepacket->pSensorData+i);
  }
  
  psentrfdata[routeprotocol+1] = routepacket->Src.Sn[0];
  psentrfdata[routeprotocol+2] = routepacket->Src.Sn[1];
  psentrfdata[routeprotocol+3] = routepacket->Des.Sn[0];
  psentrfdata[routeprotocol+4] = routepacket->Des.Sn[1];

  // routepacket->Length长度没有包含 首字节 校验和 长度字节本身
  // 计算校验和
	for( i=0;i<routepacket->Length+3;i++ )
	{
		psentrfdata[routeprotocol+5+datalength] += psentrfdata[i];
	}


		//CRPL在基站-》终端 自增1
		if ( routepacket->Key&0x80)
			routepacket->RfRouteData.CRPL++;
		else
			routepacket->RfRouteData.CRPL--;

		// 将模块Id加入路由表中
		if ( 0x81 == routepacket->Key )
		{
			//routepacket->RfRouteData.CRPL++;
	    psentrfdata[8+(routepacket->RfRouteData.CRPL-1)*2] = g_module_id.Sn[0];
	    psentrfdata[8+(routepacket->RfRouteData.CRPL-1)*2+1] = g_module_id.Sn[1];
		}	
		
		//if ( 0x01 == routepacket->Key )
			//routepacket->RfRouteData.CRPL--;


		// 提取出当前路由深度  发送唤醒波定点唤醒下级
		WorCarry[0] = psentrfdata[8+(routepacket->RfRouteData.CRPL-1)*2];
		WorCarry[1] = psentrfdata[8+(routepacket->RfRouteData.CRPL-1)*2+1];		
	
	CC1101_Wakeupcarry(WorCarry,2,2);
	halRfSendPacket(psentrfdata,routepacket->Length+3);
}

// routesize 为路由数据长度
void AssignRouteLevel(Rf_Route* routepacket,INT8U routesize)
{
  INT8U i;
  INT8U *proutedata = routepacket->RfRouteData.pRouteData	;
  
  // 存储搜索到路由标识
  if ( IapEraseByte(SEARCH_MODE,1) )
  	IapProgramByte(SEARCH_MODE,0);			
  g_search = IapReadByte(SEARCH_MODE);
  
  // EEPROM中0x0000字节开始存储搜索模式，模块ID
	if( IapEraseByte(MODEL_SN_ADDRESS,2) )
	{
		// 将具有路由级别的模块ID写入RRPROM
		IapProgramByte(MODEL_SN_ADDRESS,routepacket->Des.Sn[0]);
		IapProgramByte(MODEL_SN_ADDRESS+1,routepacket->Des.Sn[1]);
		
		if( routesize != 0 )
		{	
			// 写入路由字节长度
			IapProgramByte(MODEL_SN_ADDRESS+2,routesize);
			// 写入路由数据
			for(i=0;i<routesize;i++)
			{
				IapProgramByte(MODEL_SN_ADDRESS+i+3,*(proutedata++));
			}
		}
		// 0x82		终端响应基站分配路由级别命令
		routepacket->Key = 0x82;
		// 将数据存储到EEPROM中后，发送响应信息
		AckARL(routepacket,RfSentBuf);
	}
	else
		Log_printf("Read ID Error\n");  
  // 
  // = (routelevel<<4) | routepacket.Des.Sn[0]; // 将路由深度存储到模块ID中，建议下面有个写EEPROM的操作函数
  // WirteEeprom();
  // 将模块ID写回到EEPROM中，并存储此条路由信息
}


void TransmitDataCommand(Rf_Route* routepacket)
{
	test(routepacket,RfSentBuf);	
}

//Bool RfCheckData(INT8U *prfdata,Rf_Route routepacket,INT8U *proutedata,INT8U *pSensorData)
void RfRouteManage(INT8U *prfdata,Rf_Route* routepacket)
{
  INT8U i;
  INT8U routedatalength = 0;
  INT8U routeprotocol = 0;
  INT8U datalength = 0;
  
  routepacket->Pre = prfdata[0]; 
  routepacket->Length = prfdata[1]; 
  routepacket->Rid = prfdata[2];
  routepacket->Key = prfdata[3]; 
  
  routepacket->Gateway.Sn[0] = prfdata[4];
  routepacket->Gateway.Sn[1] = prfdata[5];
  
  // 对路由数据进行处理
  routedatalength = (routepacket->RfRouteData.Orien & 0x0F)*2;
  routepacket->RfRouteData.Orien =  prfdata[6];
  routepacket->RfRouteData.CRPL =  prfdata[7];
  // 当路由深度为0时，pRouteData不指向数组
  if( routedatalength != 0 )
    routepacket->RfRouteData.pRouteData = &prfdata[8];     // 应该将数据放到传入的
  // 计算出数据类型后的下标
  routeprotocol = routedatalength +2+6;                              // routelength+2+6 = 下一字段下标
  routepacket->ProtocolType = prfdata[routeprotocol]; 
  // 模块ID
  routepacket->Src.Sn[0] = prfdata[routeprotocol+1];
  routepacket->Src.Sn[1] = prfdata[routeprotocol+2];
  routepacket->Des.Sn[0] = prfdata[routeprotocol+3];
  routepacket->Des.Sn[1] = prfdata[routeprotocol+4];
  // 数据段长度
  datalength = routepacket->Length - ( routedatalength+2 ) - 4 - 5;     // 传感器数据长度
  if( datalength != 0 )
  	routepacket->pSensorData = &prfdata[routeprotocol+5];

// 校验和在RF接收后进行计算 在这儿可以做个赋值后的验证 
// 	for( i=0;i<routepacket->Length+3;i++ )  
//	{
//		psentrfdata[routeprotocol+5+datalength] += psentrfdata[i];
//	}

	// g_rid不同才进行数据处理 并且 源地址不同&& g_pre_mcuid.temp!= 
	if( g_pre_rid != routepacket->Rid || g_pre_src != routepacket->Src.Sn_temp )
	{
		g_pre_rid = routepacket->Rid;
		g_pre_src = routepacket->Src.Sn_temp;
		
		if( IsMe(routepacket) )
		{
		  //Key 命令字为0x01时 为分配路由级别命令 命令字为0x02时，为传输数据命令
		  switch( routepacket->Key )    
		  { // 申请路由协议时
		    case 0x01:  
		    	AssignRouteLevel(routepacket,routedatalength);
		    	break;
		    //case 0x81:  ACKARL(routepacket);
		    //case 0x82:  ACKTDC(routepacket);break;
		    default : 
		    	Log_printf("Not Route information\n");
		    	break;	
		  }
		}
		else
		{
			TransmitDataCommand(routepacket);
		}
	}
	else
	{
		Log_printf("rid equal");	
	}
}

// Key:
// 0x81		终端请求分配路由级别命令
// 0x01		基站响应终端分配路由级别命令
// 0x82		终端响应基站分配路由级别命令

// 0x03		基站请求数据命令
// 0x83		终端响应请求数据命令			



//
//void AddRoute(Rf_Route routepacket)
//{
//	routepacket->RfRouteData.CRPL++;
//	AckARL(routepacket,RfSentBuf);	
//}
//
//void SubtractRoute(Rf_Route routepacket)
//{
//	routepacket->RfRouteData.CRPL--;
//	AckARL(routepacket,RfSentBuf);
//}


//void SwitchRoute(Rf_Route* routepacket)
//{
//	// 路由方向由终端到基站 
//	if (routepacket->Key&0x80)
//	{
//		switch
//		{
//			
//  		case 0x81:
//  			AddRoute(routepacket); 
//  		break;
//  		
//  		case 2:
//  			
//  		break;
//  		
//  		default:
//  		
//  		break;			
//			
//		}
//	}	
//	else // 基站到终端
//	{
//		
//	}
//	
//}





//	psentrfdata[0] = routepacket->Pre;
//	
//	psentrfdata[2] = g_rid;										// 定义的全局变量g_rid
//	psentrfdata[3] = 0x82;										// Key
//	psentrfdata[4] = routepacket->Gateway.Sn[0];	// Gateway
//	psentrfdata[5] = routepacket->Gateway.Sn[1];
//	
//	psentrfdata[6] = (routepacket->RfRouteData.Orien&0x0F)|0x50;	// 更改传输方向
//	psentrfdata[7] = routepacket->RfRouteData.CRPL;





	//路由节点存在到基站的路由且允许直接转发给基站，
//	switch( routepacket->Key )    
//  { 
//  	// 转发基站分配路由级别命令
//		case 0x01:  
//			//SwitchRoute(routepacket);
//			SubtractRoute(routepacket);
//			break;
//		
//		// 				
//		case 0x81:	
//			;
//			break;
//			
//    default : 
//    	Log_printf("Data not for me\n");
//    	break;	
//  }










//	psentrfdata[0] = routepacket->Pre;
//	
//	psentrfdata[2] = g_rid;										// 定义的全局变量g_rid
//	psentrfdata[3] = 0x82;										// Key
//	psentrfdata[4] = routepacket->Gateway.Sn[0];	// Gateway
//	psentrfdata[5] = routepacket->Gateway.Sn[1];
//	
//	psentrfdata[6] = (routepacket->RfRouteData.Orien&0x0F)|0x50;	// 更改传输方向
//	psentrfdata[7] = routepacket->RfRouteData.CRPL;













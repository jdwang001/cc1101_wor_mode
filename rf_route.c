#include "rf_route.h"

//
// 数据结构如下所示：
// Pre Length Rid Key Gateway Route  protocoltype src des data
// 传输数据是，Rid不进行增减，Rid只是针对于路由发现的流程
// 基站进行路由级别分配时Rid = 1开始，模块的routepacket.RfPreRid = 0开始, 网关的判断应该在接收到RF数据后校验和检验正确
void AckARL(Rf_Route* routepacket,INT8U* psentrfdata)
{
	psentrfdata[0] = routepacket->Pre;
	
	psentrfdata[2] = 0x82;
	psentrfdata[3] = g_rid;
	
	
}

void AssignRouteLevel(Rf_Route* routepacket,INT8U routesize)
{
  //INT8U routelevel=0;
  INT8U *proutedata = routepacket->RfRouteData.pRouteData	;
  
  // EEPROM中0x0000字节开始存储模块ID
	if( IapEraseByte(MODEL_SN_ADDRESS,2) )
	{
		// 将具有路由界别的模块ID写入RRPROM
		IapProgramByte(MODEL_SN_ADDRESS,routepacket->Des.Sn[0]);
		IapProgramByte(MODEL_SN_ADDRESS+1,routepacket->Des.Sn[1]);
		if( routesize != 0 )
		{	
			// 写入路由字节长度
			IapProgramByte(MODEL_SN_ADDRESS+2,routesize);
			// 写入路由数据
			for(i=0;i<routesize;i++)
			{
				IapProgramByte(MODEL_SN_ADDRESS+i+3,proutedata++);
			}
			
		}
	}
	else
		Log_printf("Read ID Error\n");  
  // 
   //= (routelevel<<4) | routepacket.Des.Sn[0]; // 将路由深度存储到模块ID中，建议下面有个写EEPROM的操作函数
  //WirteEeprom();
  // 将模块ID写回到EEPROM中，并存储此条路由信息
}

// 唤醒分为两种，广播和模块唤醒 此处处理是否是模块唤醒
INT8U IsMe(Rf_Route* routepacket)
{
  // 判断 目的地址 和  模块地址 是否相同
  if( ( g_module_id.Sn_temp&0x0FFF ) == ( routepacket->Des.Sn_temp&0x0FFF ) )
    return 1;
  else
    return 0;
}

//Bool RfCheckData(INT8U *prfdata,Rf_Route routepacket,INT8U *proutedata,INT8U *pSensorData)
INT8U RfRouteManage(INT8U *prfdata,Rf_Route* routepacket)
{
  INT8U routedatalength = 0;
  INT8U routeprotocol = 0;
  INT8U datalength = 0;
  
  routepacket->Pre = prfdata[0]; 
  routepacket->Length = prfdata[1]; 
  routepacket->Rid = prfdata[2];
  routepacket->Key = prfdata[3]; 
  routepacket->Gateway = prfdata[4];
  
  // 对路由数据进行处理
  routedatalength = (routepacket->RfRouteData.Orien & 0x0F)*2;
  routepacket->RfRouteData.Orien =  prfdata[5];
  routepacket->RfRouteData.CRPL =  prfdata[6];
  // 当路由深度为0时，pRouteData不指向数组
  if( routedatalength != 0 )
    routepacket->RfRouteData.pRouteData = &prfdata[7];     // 应该将数据放到传入的
  // 计算出数据类型后的下标
  routeprotocol = routedatalength +2+5;                              // routelength+2+5 = 下一字段下标
  routepacket->ProtocolType = prfdata[routeprotocol]; 
  // 模块ID
  routepacket->Src.Sn[0] = prfdata[routeprotocol+1];
  routepacket->Src.Sn[1] = prfdata[routeprotocol+2];
  routepacket->Des.Sn[0] = prfdata[routeprotocol+3];
  routepacket->Des.Sn[1] = prfdata[routeprotocol+4];
  // 数据段长度
  datalength = routepacket->Length - ( routedatalength+2 ) - 4 - 4;     // 传感器数据长度
  routepacket->pSensorData = &prfdata[routeprotocol+5];
 
	if( IsMe(routepacket) )
	{
	  //Key 命令字为0x01时 为分配路由级别命令 命令字为0x02时，为传输数据命令
	  switch(routepacket->Key)    
	  { // 申请路由协议时
	    case 0x01:  AssignRouteLevel(routepacket);
	    //case 0x81:  ACKARL(routepacket);
	    case 0x02:  TransmitDataCommand(routepacket);
	    case 0x82:  ACKTDC(routepacket);
	    default : Log_printf("Not Route information\n");
	  }
	}
	else
	{
		
	}
}

























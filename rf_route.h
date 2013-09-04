#ifndef _RF_ROUTE_H__
#define _RF_ROUTE_H__


#include "rf_config.h"

typedef struct RF_ROUTE_DATA{
 INT8U Orien;
 INT8U CRPL;					// 当前路由深度	Current Route Protocols Level
 INT8U *pRouteData;
} Rf_Route_Data;

typedef struct RF_ROUTE{
 INT8U Pre;
 INT8U Length;
 INT8U Rid;
 INT8U Key;
 Module_Sn Gateway;
 INT8U ProtocolType;
 INT8U	CheckSum;
 
 Module_Sn Src;
 Module_Sn Des;
 Rf_Route_Data RfRouteData;
 INT8U *pSensorData;
} Rf_Route;

//Rf_Route rf_route;				// 将接收到的数据映射到此结构体中，进行解析
//INT8U RouteData[6];				// 路由数据
//INT8U SensorData[10];			// 传感器数据
extern INT8U RfSentBuf[64];
extern INT8U RfRecBuf[64];
extern INT8U g_rid,g_pre_rid;
extern INT16U g_pre_src;
extern INT8U g_getroute;
extern Rf_Route rf_route_data;
extern INT8U g_route_size;

INT8U CheckRouteData(INT8U *prfdata,Rf_Route* routepacket);
void AssignRouteLevel(Rf_Route* routepacket);
void AckARL(Rf_Route* routepacket,INT8U* psentrfdata);
void TransmitDataCommand(Rf_Route* routepacket);
void ACKTDC(Rf_Route* routepacket);
INT8U RfCheckData(INT8U* prfdata,Rf_Route* routedata);
void RfRouteManage(Rf_Route* routedata);
void DirectTransmitDataCommand(Rf_Route* routepacket);

#endif

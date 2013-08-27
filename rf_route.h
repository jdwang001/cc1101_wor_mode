#ifndef _RF_ROUTE_H__
#define _RF_ROUTE_H__


#include "rf_config.h"

typedef struct RF_ROUTE_DATA{
 INT8U Orien;
 INT8U CRPL;					// ��ǰ·�����	Current Route Protocols Level
 INT8U *pRouteData;
} Rf_Route_Data;

typedef struct RF_ROUTE{
 INT8U Pre;
 INT8U Length;
 INT8U Rid;
 INT8U Key;
 INT8U Gateway;
 INT8U ProtocolType;
 INT8U	CheckSum;
 
 Module_Sn Src;
 Module_Sn Des;
 Rf_Route_Data RfRouteData;
 INT8U *pSensorData;
} Rf_Route;



//Rf_Route rf_route;				// �����յ�������ӳ�䵽�˽ṹ���У����н���
//INT8U RouteData[6];				// ·������
//INT8U SensorData[10];			// ����������

void AssignRouteLevel(Rf_Route* routepacket);
void AckARL(Rf_Route* routepacket);
void TransmitDataCommand(Rf_Route* routepacket);
void ACKTDC(Rf_Route* routepacket);
INT8U RfCheckData(INT8U* prfdata,Rf_Route* routedata);
INT8U RfRouteManage(INT8U* prfdata,Rf_Route* routedata);


#endif

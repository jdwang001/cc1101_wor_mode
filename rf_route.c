#include "rf_route.h"

//
// ���ݽṹ������ʾ��
// Pre Length Rid Key Gateway Route  protocoltype src des data
// ���������ǣ�Rid������������Ridֻ�������·�ɷ��ֵ�����
// ��վ����·�ɼ������ʱRid = 1��ʼ��ģ���routepacket.RfPreRid = 0��ʼ, ���ص��ж�Ӧ���ڽ��յ�RF���ݺ�У��ͼ�����ȷ
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
  
  // EEPROM��0x0000�ֽڿ�ʼ�洢ģ��ID
	if( IapEraseByte(MODEL_SN_ADDRESS,2) )
	{
		// ������·�ɽ���ģ��IDд��RRPROM
		IapProgramByte(MODEL_SN_ADDRESS,routepacket->Des.Sn[0]);
		IapProgramByte(MODEL_SN_ADDRESS+1,routepacket->Des.Sn[1]);
		if( routesize != 0 )
		{	
			// д��·���ֽڳ���
			IapProgramByte(MODEL_SN_ADDRESS+2,routesize);
			// д��·������
			for(i=0;i<routesize;i++)
			{
				IapProgramByte(MODEL_SN_ADDRESS+i+3,proutedata++);
			}
			
		}
	}
	else
		Log_printf("Read ID Error\n");  
  // 
   //= (routelevel<<4) | routepacket.Des.Sn[0]; // ��·����ȴ洢��ģ��ID�У����������и�дEEPROM�Ĳ�������
  //WirteEeprom();
  // ��ģ��IDд�ص�EEPROM�У����洢����·����Ϣ
}

// ���ѷ�Ϊ���֣��㲥��ģ�黽�� �˴������Ƿ���ģ�黽��
INT8U IsMe(Rf_Route* routepacket)
{
  // �ж� Ŀ�ĵ�ַ ��  ģ���ַ �Ƿ���ͬ
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
  
  // ��·�����ݽ��д���
  routedatalength = (routepacket->RfRouteData.Orien & 0x0F)*2;
  routepacket->RfRouteData.Orien =  prfdata[5];
  routepacket->RfRouteData.CRPL =  prfdata[6];
  // ��·�����Ϊ0ʱ��pRouteData��ָ������
  if( routedatalength != 0 )
    routepacket->RfRouteData.pRouteData = &prfdata[7];     // Ӧ�ý����ݷŵ������
  // ������������ͺ���±�
  routeprotocol = routedatalength +2+5;                              // routelength+2+5 = ��һ�ֶ��±�
  routepacket->ProtocolType = prfdata[routeprotocol]; 
  // ģ��ID
  routepacket->Src.Sn[0] = prfdata[routeprotocol+1];
  routepacket->Src.Sn[1] = prfdata[routeprotocol+2];
  routepacket->Des.Sn[0] = prfdata[routeprotocol+3];
  routepacket->Des.Sn[1] = prfdata[routeprotocol+4];
  // ���ݶγ���
  datalength = routepacket->Length - ( routedatalength+2 ) - 4 - 4;     // ���������ݳ���
  routepacket->pSensorData = &prfdata[routeprotocol+5];
 
	if( IsMe(routepacket) )
	{
	  //Key ������Ϊ0x01ʱ Ϊ����·�ɼ������� ������Ϊ0x02ʱ��Ϊ������������
	  switch(routepacket->Key)    
	  { // ����·��Э��ʱ
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

























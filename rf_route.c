// Filename:	rf_route.c
// Function:	rf·�ɲ����ݴ���
// Author:		wzd
// Date:			2013��8��27��22:43:24


#include "rf_route.h"

// ���ѷ�Ϊ���֣��㲥��ģ�黽�� �˴������Ƿ���ģ�黽�ѣ�������������������
// �ж������Ƿ��Ƿ��͸���ģ��
INT8U IsMe(Rf_Route* routepacket)
{
  // �ж� Ŀ�ĵ�ַ ��  ģ���ַ �Ƿ���ͬ  �޳�·����Ϣ ���ֳ� ģ��ͻ�վ ģ�����λΪ0 ��վ���λΪ1
  if( ( g_module_id.Sn_temp&0x8FFF ) == ( routepacket->Des.Sn_temp&0x8FFF ) )
    return 1;
  else
    return 0;
}

//
// ���ݽṹ������ʾ��
// Pre Length Rid Key Gateway Route  protocoltype src des data
// ���������ǣ�Rid������������Ridֻ�������·�ɷ��ֵ�����
// ��վ����·�ɼ������ʱRid = 1��ʼ��ģ���routepacket.RfPreRid = 0��ʼ, ���ص��ж�Ӧ���ڽ��յ�RF���ݺ�У��ͼ�����ȷ
void AckARL(Rf_Route* routepacket,INT8U* psentrfdata)
{
	// ��ֱ�ӽ�ȡԭ�ṹ�����ݣ����ظ���վ
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
  
  // ��·�����ݽ��д���
  routedatalength = (routepacket->RfRouteData.Orien & 0x0F)*2;
  psentrfdata[6] = (routepacket->RfRouteData.Orien&0x0F)|0x50;		// ���ķ���
  psentrfdata[7] = routepacket->RfRouteData.CRPL;
  // ��·�����Ϊ0ʱ��pRouteData��ָ������
  if( routedatalength != 0 )
  {
    //routepacket->RfRouteData.pRouteData = &psentrfdata[8];     // Ӧ�ý����ݷŵ������
    for(i=0;i<routedatalength;i++)
    	psentrfdata[8+i] = *(routepacket->RfRouteData.pRouteData+i);
  }
  // ������������ͺ���±�
  routeprotocol = routedatalength +2+6;                              // routelength+2+6 = ��һ�ֶ��±�
  psentrfdata[routeprotocol] = routepacket->ProtocolType; 
  // �ش�����ʱ��Src��Des����λ��
  psentrfdata[routeprotocol+1] = routepacket->Des.Sn[0];
  psentrfdata[routeprotocol+2] = routepacket->Des.Sn[1];
  psentrfdata[routeprotocol+3] = routepacket->Src.Sn[0];
  psentrfdata[routeprotocol+4] = routepacket->Src.Sn[1];
  // ���ݶγ���
  datalength = routepacket->Length - ( routedatalength+2 ) - 4 - 5;     // ���������ݳ���
  if( datalength != 0 )
  {
  	for( i=0;i<datalength;i++ )
  		psentrfdata[routeprotocol+5+i] = *(routepacket->pSensorData+i);
  }
  // routepacket->Length����û�а��� ���ֽ� У��� �����ֽڱ���
  // ����У���
	for( i=0;i<routepacket->Length+3;i++ )  
	{
		psentrfdata[routeprotocol+5+datalength] += psentrfdata[i];
	}
	// ��ȡ����ǰ·�����  ���ͻ��Ѳ����㻽���¼�
	WorCarry[0] = psentrfdata[8+(routepacket->RfRouteData.CRPL-1)*2];
	WorCarry[1] = psentrfdata[8+(routepacket->RfRouteData.CRPL-1)*2+1];
	
	CC1101_Wakeupcarry(WorCarry,2,2);
	halRfSendPacket(psentrfdata,routepacket->Length+3);
	g_rid++;
}

void test(Rf_Route* routepacket,INT8U* psentrfdata)
{
	// ��ֱ�ӽ�ȡԭ�ṹ�����ݣ����ظ���վ
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
  	psentrfdata[6] = routepacket->RfRouteData.Orien+1;		// ����Ѱ·�ɴ������վʱ����1
  else
  	psentrfdata[6] = routepacket->RfRouteData.Orien;
  
  // ��·�����ݽ��д���OrienΪ0 ʱ  CRPLΪ1
  routedatalength = (routepacket->RfRouteData.Orien & 0x0F)*2;
  psentrfdata[7] = routepacket->RfRouteData.CRPL;
  // ��·�����Ϊ0ʱ��pRouteData��ָ������
 
  if( routedatalength != 0 )
  {
    //routepacket->RfRouteData.pRouteData = &psentrfdata[8];     // Ӧ�ý����ݷŵ������
    for(i=0;i<routedatalength;i++)
    	psentrfdata[8+i] = *(routepacket->RfRouteData.pRouteData+i);
  }
  
  
  // ������������ͺ���±�
  routeprotocol = routedatalength +2+6;                              // routelength+2+6 = ��һ�ֶ��±�
  psentrfdata[routeprotocol] = routepacket->ProtocolType; 
  // ���ݶγ���
  datalength = routepacket->Length - ( routedatalength+2 ) - 4 - 5;     // ���������ݳ���
  if( datalength != 0 )
  {
  	for( i=0;i<datalength;i++ )
  		psentrfdata[routeprotocol+5+i] = *(routepacket->pSensorData+i);
  }
  
  psentrfdata[routeprotocol+1] = routepacket->Src.Sn[0];
  psentrfdata[routeprotocol+2] = routepacket->Src.Sn[1];
  psentrfdata[routeprotocol+3] = routepacket->Des.Sn[0];
  psentrfdata[routeprotocol+4] = routepacket->Des.Sn[1];

  // routepacket->Length����û�а��� ���ֽ� У��� �����ֽڱ���
  // ����У���
	for( i=0;i<routepacket->Length+3;i++ )
	{
		psentrfdata[routeprotocol+5+datalength] += psentrfdata[i];
	}


		//CRPL�ڻ�վ-���ն� ����1
		if ( routepacket->Key&0x80)
			routepacket->RfRouteData.CRPL++;
		else
			routepacket->RfRouteData.CRPL--;

		// ��ģ��Id����·�ɱ���
		if ( 0x81 == routepacket->Key )
		{
			//routepacket->RfRouteData.CRPL++;
	    psentrfdata[8+(routepacket->RfRouteData.CRPL-1)*2] = g_module_id.Sn[0];
	    psentrfdata[8+(routepacket->RfRouteData.CRPL-1)*2+1] = g_module_id.Sn[1];
		}	
		
		//if ( 0x01 == routepacket->Key )
			//routepacket->RfRouteData.CRPL--;


		// ��ȡ����ǰ·�����  ���ͻ��Ѳ����㻽���¼�
		WorCarry[0] = psentrfdata[8+(routepacket->RfRouteData.CRPL-1)*2];
		WorCarry[1] = psentrfdata[8+(routepacket->RfRouteData.CRPL-1)*2+1];		
	
	CC1101_Wakeupcarry(WorCarry,2,2);
	halRfSendPacket(psentrfdata,routepacket->Length+3);
}

// routesize Ϊ·�����ݳ���
void AssignRouteLevel(Rf_Route* routepacket,INT8U routesize)
{
  INT8U i;
  INT8U *proutedata = routepacket->RfRouteData.pRouteData	;
  
  // �洢������·�ɱ�ʶ
  if ( IapEraseByte(SEARCH_MODE,1) )
  	IapProgramByte(SEARCH_MODE,0);			
  g_search = IapReadByte(SEARCH_MODE);
  
  // EEPROM��0x0000�ֽڿ�ʼ�洢����ģʽ��ģ��ID
	if( IapEraseByte(MODEL_SN_ADDRESS,2) )
	{
		// ������·�ɼ����ģ��IDд��RRPROM
		IapProgramByte(MODEL_SN_ADDRESS,routepacket->Des.Sn[0]);
		IapProgramByte(MODEL_SN_ADDRESS+1,routepacket->Des.Sn[1]);
		
		if( routesize != 0 )
		{	
			// д��·���ֽڳ���
			IapProgramByte(MODEL_SN_ADDRESS+2,routesize);
			// д��·������
			for(i=0;i<routesize;i++)
			{
				IapProgramByte(MODEL_SN_ADDRESS+i+3,*(proutedata++));
			}
		}
		// 0x82		�ն���Ӧ��վ����·�ɼ�������
		routepacket->Key = 0x82;
		// �����ݴ洢��EEPROM�к󣬷�����Ӧ��Ϣ
		AckARL(routepacket,RfSentBuf);
	}
	else
		Log_printf("Read ID Error\n");  
  // 
  // = (routelevel<<4) | routepacket.Des.Sn[0]; // ��·����ȴ洢��ģ��ID�У����������и�дEEPROM�Ĳ�������
  // WirteEeprom();
  // ��ģ��IDд�ص�EEPROM�У����洢����·����Ϣ
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
  
  // ��·�����ݽ��д���
  routedatalength = (routepacket->RfRouteData.Orien & 0x0F)*2;
  routepacket->RfRouteData.Orien =  prfdata[6];
  routepacket->RfRouteData.CRPL =  prfdata[7];
  // ��·�����Ϊ0ʱ��pRouteData��ָ������
  if( routedatalength != 0 )
    routepacket->RfRouteData.pRouteData = &prfdata[8];     // Ӧ�ý����ݷŵ������
  // ������������ͺ���±�
  routeprotocol = routedatalength +2+6;                              // routelength+2+6 = ��һ�ֶ��±�
  routepacket->ProtocolType = prfdata[routeprotocol]; 
  // ģ��ID
  routepacket->Src.Sn[0] = prfdata[routeprotocol+1];
  routepacket->Src.Sn[1] = prfdata[routeprotocol+2];
  routepacket->Des.Sn[0] = prfdata[routeprotocol+3];
  routepacket->Des.Sn[1] = prfdata[routeprotocol+4];
  // ���ݶγ���
  datalength = routepacket->Length - ( routedatalength+2 ) - 4 - 5;     // ���������ݳ���
  if( datalength != 0 )
  	routepacket->pSensorData = &prfdata[routeprotocol+5];

// У�����RF���պ���м��� ���������������ֵ�����֤ 
// 	for( i=0;i<routepacket->Length+3;i++ )  
//	{
//		psentrfdata[routeprotocol+5+datalength] += psentrfdata[i];
//	}

	// g_rid��ͬ�Ž������ݴ��� ���� Դ��ַ��ͬ&& g_pre_mcuid.temp!= 
	if( g_pre_rid != routepacket->Rid || g_pre_src != routepacket->Src.Sn_temp )
	{
		g_pre_rid = routepacket->Rid;
		g_pre_src = routepacket->Src.Sn_temp;
		
		if( IsMe(routepacket) )
		{
		  //Key ������Ϊ0x01ʱ Ϊ����·�ɼ������� ������Ϊ0x02ʱ��Ϊ������������
		  switch( routepacket->Key )    
		  { // ����·��Э��ʱ
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
// 0x81		�ն��������·�ɼ�������
// 0x01		��վ��Ӧ�ն˷���·�ɼ�������
// 0x82		�ն���Ӧ��վ����·�ɼ�������

// 0x03		��վ������������
// 0x83		�ն���Ӧ������������			



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
//	// ·�ɷ������ն˵���վ 
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
//	else // ��վ���ն�
//	{
//		
//	}
//	
//}





//	psentrfdata[0] = routepacket->Pre;
//	
//	psentrfdata[2] = g_rid;										// �����ȫ�ֱ���g_rid
//	psentrfdata[3] = 0x82;										// Key
//	psentrfdata[4] = routepacket->Gateway.Sn[0];	// Gateway
//	psentrfdata[5] = routepacket->Gateway.Sn[1];
//	
//	psentrfdata[6] = (routepacket->RfRouteData.Orien&0x0F)|0x50;	// ���Ĵ��䷽��
//	psentrfdata[7] = routepacket->RfRouteData.CRPL;





	//·�ɽڵ���ڵ���վ��·��������ֱ��ת������վ��
//	switch( routepacket->Key )    
//  { 
//  	// ת����վ����·�ɼ�������
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
//	psentrfdata[2] = g_rid;										// �����ȫ�ֱ���g_rid
//	psentrfdata[3] = 0x82;										// Key
//	psentrfdata[4] = routepacket->Gateway.Sn[0];	// Gateway
//	psentrfdata[5] = routepacket->Gateway.Sn[1];
//	
//	psentrfdata[6] = (routepacket->RfRouteData.Orien&0x0F)|0x50;	// ���Ĵ��䷽��
//	psentrfdata[7] = routepacket->RfRouteData.CRPL;













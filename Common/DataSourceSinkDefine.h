/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DataSourceDefine.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	��������ص��ӿڶ���ͷ�ļ�
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-7-14
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma  once

#include "NetEngineInterface.h"

enum CommNotifyType  //״̬֪ͨ����
{
	eDataSource_ReConnect = 1,
	eDataSource_Init = 2
};

struct CommNotify
{
	CommNotifyType       m_uType; 
	Server_type               m_svrType;
	char				 m_strMsg[256];
};

class IDataSourceSink
{
public:
	/********************************************************************************
	* �������� :��������ͨѶ�ص�
	* �������� :void* pData - ָ��һ��CNI_General�ṹ��ָ�룬�������ͨѶ״̬
	*  ����ֵ  :TRUE �ɹ�		
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual BOOL HSDataSourceSink_OnCommNotify(void* pData) = 0;

	/********************************************************************************
	* �������� :�����������ݻص� (��Խ���ģ�飬�ù��̽����������)
	* �������� :void* pData - ָ��һ��CNI_ReceiveData�ṹ��ָ��
	*			int nLen - ���ݳ���
	*  ����ֵ  :TRUE �ɹ�		
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual BOOL HSDataSourceSink_OnRecvData(void* pData, int nLen) = 0;

};

class IDataSourceSinkEx : public IDataSourceSink
{
public:
	/********************************************************************************
	* �������� :����������ý���ģ�� ͬ�������̴߳���
	* �������� :
	*			
	*  ����ֵ  :	
	* �������� :	HSDataSourceSink_OnRecvData
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual BOOL HSDataSourceSinkEx_RecvDataNotify() = 0;
};
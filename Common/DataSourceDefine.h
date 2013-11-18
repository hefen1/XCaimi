/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DataSourceDefine.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	��������ӿڶ���ͷ�ļ�
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-7-13
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma  once

#include "DataSourceSinkDefine.h"

enum{
	e_DataSouceSend_HQData = 0,			//��������
	e_DataSouceSend_JYData = 0x01,		//��������
	
	e_DataSouceSend_High = 0x010,		//�����ȼ�
	e_DataSouceSend_Normal = 0x020,
	e_DataSouceSend_Low = 0x040,

	e_DataSouceSend_Add = 0x0100,		//��ͨ���ӵĲ���
	e_DataSouceSend_Clear = 0x0200,		//��հ�

	e_DataSouceSend_ReadDisk = 0x1000,	//������

	e_DataSourceSend_HQDataDownLoad = 0x2000,  //������������
};

#define OBJ_HSDATAENGINE	_T("DataSource.HSDSEngine")

struct CDataSourceInitDataStauts 
{
	char	m_szMsg[256];
};

typedef void(WINAPI *PFN_INITSTATUS_NOTIFY)(CDataSourceInitDataStauts*, CWnd* pWnd); 

class IDataSource
{
public:
	/********************************************************************************
	* �������� :������������ص����
	* �������� :IDataSourceSink* pSink - �ص��ľ��
	*			 const char* pszProfile - 
	*  ����ֵ  :����������õľ�� long		
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual long HSDataSource_DataSouceInit(IN IDataSourceSink* pSink, IN const char* pszProfile = NULL) = 0;


	/********************************************************************************
	* ��������:��������ע��
	* ��������:IDataSourceSink* pSink - �ص��ľ��
	*			  
	*  ����ֵ :TRUE�ɹ� 
	*  		
	* ��������:	HSDataSource_DataSouceInit
	* ��   ��:
	* �������:
	* �޸ļ�¼:
	*******************************************************************************/
	virtual BOOL HSDataSource_DataSourceUnInit(IN long lDataSourceID) = 0;


	/********************************************************************************
	* ��������:�첽�������������ͺ��������أ����������ڻص��и���
	* ��������:long lDataSourceID - HSDataSource_SetCommInit�ķ��ؽ��
	*			
	*			char* pAskData - ��������� ---- �������Ϊ�գ�����Ϊ0ʱ����Ϊ�Ǵ�lDataSourceID ��Ҫһ����հ���ȡ�����ж���
	*			int nAskLen - �������С
	*			int nTimeOut - ��ʱʱ��
	*			UINT uSendPriority = e_DataSouceSend_Normal - ���ݷ���ѡ��������ȼ�����հ���
	*			BOOL bReadDisk              �Ƿ������
	*			long lConnectHandle = 0 - ���Ӿ��
	*			long lPackID - �����
	*  ����ֵ :TRUE �ɹ�	
	* ��������: HSDataSource_SetCommInit	
	* ��   ��:
	* �������:
	* �޸ļ�¼: 
	*******************************************************************************/

	virtual BOOL HSDataSource_RequestAsyncData(IN long lDataSourceID, IN char* pAskData, IN int nAskLen, IN int nTimeOut, 
		IN UINT uSendOptions = e_DataSouceSend_Normal, IN long lConnectHandle = 0, IN long lPackID = 0) = 0;

	/********************************************************************************
	* ��������:ͬ��������������Ӧ����ٷ��ؽ��
	* ��������:long lDataSourceID - HSDataSource_SetCommInit�ķ���ֵ
	*			
	*			char* pAskData - ���������
	*			int nAskLen - �������С
	*			int nTimeOut - ��ʱʱ��
	*			int& pLen - ���ذ���С
	*			UINT uSendPriority = e_DataSouceSend_Normal - ���ݷ���ѡ��������ȼ�����հ���
	*			long lConnectHandle = 0 - ���Ӿ��
	*			long lPackID - �����
	*  ����ֵ :	���ݰ�ָ��	
	* ��������: HSDataSource_SetCommInit	
	* ��   ��:
	* �������:
	* �޸ļ�¼:
	*******************************************************************************/
	virtual void* HSDataSource_RequestSyncData(IN long lDataSourceID, IN char* pAskData, IN int nAskLen, IN int nTimeOut, 
		IN OUT int& pLen, IN UINT uSendOptions = e_DataSouceSend_Normal, IN long lConnectHandle = 0, IN long lPackID = 0) = 0;

	/********************************************************************************
	* ��������:ͬ�����ݴ���������ڴ������ɾ��
	* ��������:long lDataSourceID - HSDataSource_SetCommInit�ķ���ֵ
	*			
	*			void* pRefData - ͬ�����ݷ��صĽ��ָ��
	*			int nRefDataLen - ���ݰ���С
	*			long lConnectHandle - ���Ӿ��
	*  ����ֵ :TRUE �ɹ�	
	* ��������: HSDataSource_RequestSyncData	
	* ��   ��:
	* �������:
	* �޸ļ�¼:
	*******************************************************************************/
	virtual BOOL HSDataSource_SyncDataRefComplete(IN long lDataSourceID, IN void* pRefData, IN int nRefDataLen, IN long lConnectHandle = 0) = 0;

	/********************************************************************************
	* ��������:��ȡ����������
	* ��������:long lConnectHandle - ���Ӿ��
	*			
	*			
	*			
	*			
	*  ����ֵ :��������	
	* ��������: HSDataSource_RequestSyncData	
	* ��   ��:
	* �������:
	* �޸ļ�¼:
	*******************************************************************************/
	virtual CString	HSDataSource_GetLastError(IN long lConnectHandle = 0) = 0;

	/********************************************************************************
	* ��������:��¼���ⲿ����ģ������������棬�����ݳ�ʼ��
	* ��������:
	*			
	*			
	*			
	*			
	*  ����ֵ : 0 �ɹ�
	* ��������: HSDataSource_RequestSyncData	
	* ��   ��:
	* �������:
	* �޸ļ�¼:
	*******************************************************************************/
	virtual long	HSDataSource_ServerDataInit(IN long lConnectHandle, IN PFN_INITSTATUS_NOTIFY fnnotify, IN CWnd* pWnd) = 0;
	virtual void    HSDataSource_SetDefaultConnectHandle(IN long lConnectHandle) = 0;
	/********************************************************************************
	* ��������:��¼���ⲿ����ģ������������棬����֪ͨ
	* ��������:
	*			
	*			
	*			
	*			
	*  ����ֵ : 0 �ɹ�
	* ��������: HSDataSource_ReConnectNotify	
	* ��   ��:
	* �������:
	* �޸ļ�¼:
	*******************************************************************************/
	virtual BOOL HSDataSource_ReConnectNotify(IN long lConnectHandle) = 0;
};
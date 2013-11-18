/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DataSinkConnectManager.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	��������ע��ص�����Ӧ���ӹ�����
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-9-1
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma  once
struct CDataSourceSinkToConnectID
{
	long		m_lConnectHandle;				//ͨѶ���Ӿ��
	long		m_lDataSinkID;					//�ص�
	long		m_lPackID;						//�����
	UINT		m_uSinkType;					//�������� - ���ף�����

	CDataSourceSinkToConnectID()
	{
		memset(this, 0, sizeof(CDataSourceSinkToConnectID));
	}
};

class CDataSinkConnectManager
{
protected:
	//���ػ����Ӧ��ϵ����
	std::list<CDataSourceSinkToConnectID*>	m_listData;
	//�����Ӧ��ϵ������
	CMutex									m_mutex;	

public:
	//	���캯������������
	CDataSinkConnectManager();
	~CDataSinkConnectManager();

public:
	BOOL							AddUpdateNew(long lDataSinkID, long lConnectHandle, long lPackID = 0, UINT uSinkType = e_DataSouceSend_HQData);
	long							RemoveSink(long lDataSinkID);
	int								GetSize();
	BOOL							RemoveAllElement();
	CDataSourceSinkToConnectID*		GetDataSinkID(long lConnectHandle, long lPackID = 0);
	BOOL							SendSinkData(long lSinkID, void* pszDataBuff, int nDataLen);
	long							SendSinkData(long lConnectHandle, long lPackID, void* pszDataBuff, int nDataLen);
	BOOL							NotifySinkData(long lSinkID, void* pszDataBuff, int nDataLen);
	BOOL							NotifyCommStatus(void* pszDataBuff,long lSinkID=-1);
	BOOL							SendAllInitFinishInfo();
};
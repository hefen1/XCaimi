#pragma once
#include <afxmt.h>
#include "NetEngineStructDefine.h"

class CConnectorManagerImp: public IChannelManager
{
public:
	CConnectorManagerImp(void);
	~CConnectorManagerImp(void);
public:
	/********************************************************************************
	* �������� :����ָ������
	* �������� :Server_type cType: ��������
	*           CString strName: ����������(�ձ�ʾĬ��վ��)
	*           �ɹ�����Connector�ӿ�ָ�룬��֮ΪNULL
	*  ����ֵ  :�ɹ�����0����֮ΪFALSE	
	*******************************************************************************/
	virtual long CreateConnector(Server_type cType,CString strName,TradeCert *ptr = NULL) ;

	/********************************************************************************
	* �������� :������������
	* �������� :Server_type cType: ��������
	*  ����ֵ  :�ɹ�����Connector�ӿ�ָ�룬��֮ΪNULL	
	*******************************************************************************/
	virtual long CreateBestConnector(Server_type cType,TradeCert *ptr = NULL);


	/********************************************************************************
	* �������� :������������
	* �������� :Connector *pConnect: Creator���ص����ӽӿ�
	*  ����ֵ  :�ɹ�����0
	*******************************************************************************/
	virtual int DestroyConnect(long handle);

	/********************************************************************************
	* �������� :����
	* �������� :Server_type cType: ��Ҫ���ٵķ���������
	* �������� :TestSpeedCallBack tsCall:���ٻص����Է��ز�����Ϣ
	*  ����ֵ  :�ɹ�����0
	* �ڵ��øú���ǰ�����ȵ���SetOnlineRadioRange��SetHandleTimeRange����Ȩ��
	*******************************************************************************/
	virtual int TestSiteSpeed(Server_type cType,TestSpeedCallBack tsCall,void *ptr);

	/********************************************************************************
	* �������� :ֹͣ����
	*  ����ֵ  :�ɹ�����0
	*******************************************************************************/
	virtual int StopSiteTest(Server_type cType);


	/********************************************************************************
	* �������� :ע��֪ͨ�ص�
	* �������� :OnConnectNotify notify:֪ͨ�ص�����
	*  ����ֵ  :�ɹ�����0
	*******************************************************************************/
	virtual int RegisterNotify(OnConnectNotify notify);


	/********************************************************************************
	* �������� :ȡ��ע��֪ͨ�ص�
	* �������� :OnConnectNotify notify:֪ͨ�ص�����
	*  ����ֵ  :�ɹ�����0
	*******************************************************************************/
	virtual int UnregisterNotify(OnConnectNotify notify);
	/********************************************************************************
	* �������� :���ò��ٽ��������ط�Χ
	* �������� :Server_type cType: ����������
	* �������� :RangeItem����ָ��
	* �������� :RangeItem����Ԫ�ظ���
	*******************************************************************************/
	virtual void SetOnlineRadioRange(Server_type cType,RangeItem *pOnline,int count);
	virtual void SetHandleTimeRange(Server_type cType,RangeItem *pTime,int count);
	/********************************************************************************
	* �������� :���÷�������Ϣ
	* �������� :CommInfo����ָ��
	* �������� :����Ԫ�ظ���
	* �������� :ProxyInfo�ṹָ��
	* ����������typeҪ���õķ��������ͣ�Ĭ�������з�����
	*********************************************************************************/
	virtual int SetSites(CommInfo *pComm, int count,ProxyInfo *pProxy,Server_type type = CEV_Connect_ALL);
	/********************************************************************************
	* �������� :���Դ��������
	* �������� :ProxyInfo�ṹָ��
	* ����ֵ���ɹ�����0������-1
	********************************************************************************/
	virtual int TestProxy(ProxyInfo *pProxy);
	/********************************************************************************
	* �������� :���ݾ����ȡվ������
	* �������� :���Ӿ��
	* ����ֵ������վ������
	********************************************************************************/
	virtual CString GetServerName(long handle);

	/********************************************************************************
	* �������� :���ݾ����ȡMAC��ַ
	* �������� :���Ӿ��
	* ����ֵ������MAC��ַ
	********************************************************************************/
	virtual CString GetMacAdd(long handle);
	/********************************************************************************
	* �������� :������һ��վ��
	* �������� : ǰһ��վ���վ����
	* ����ֵ�����Ӿ��
	********************************************************************************/
	virtual long ConnectNextSite(Server_type cType,CString strName,TradeCert *ptr);

public:
	bool IsConnected(long handle);
	BOOL IsConnectHandle(long handle);
	float GetOnlineRadioRange(Server_type cType,float fValue);
	float GetHandleTimeRange(Server_type cType,float fValue);
	BOOL  Notify(ConnectNotify *pNot);
	static BOOL IsHQServerType(Server_type type);  //�Ƿ�������������
	BOOL FConnNotify(ConnectNotify *pnot);

protected:
	void DestroyRemovedConn();//ͨѶ���ú��п��ܽ���ǰ����ɾ����,�Ƴ���ɾ��������
	void Free();

protected:
	ServerInfo    m_siteInfoList;      //����վ����Ϣ�б�
	CArray<OnConnectNotify,OnConnectNotify> m_notifyList; //����״̬֪ͨ�ص��б�
	CArray<long,long> m_connectedSiteList;   //��ǰ������Ϣ�б�
	CArray<long,long> m_disConnectList;     //�Ͽ��������б�
	CCriticalSection m_cCritical_DisConn;
	CCriticalSection m_cCritical_Conn;       //�ٽ�������
	CCriticalSection m_cCritical_Notify;

protected:
	CMapWordToPtr m_mapSpeedTest; //������ٶ���ÿ������������һ��CServerSpeedTest��ʵ��
	CMapWordToPtr m_speedTestRange;//������ټ����õ��ı���

protected:
	CMap<long,long,void*,void*> m_mapHeartBeat;//key:����handle��value:��������
	CCriticalSection m_cCritical_Heart;       //�ٽ�������
	CArray<ConnectNotify*,ConnectNotify*> m_ayConnNotify;//����״̬֪ͨ
	CCriticalSection m_cCritical_ConnNotify;       //�ٽ�������
	HANDLE m_hEventConnNotify;
	HANDLE m_hThreadCheck;
	void CreateHeartBeat(long handle);
	void StopHeartBeat(long handle);
	void DeleteConnect(long handle);
	void RemoveDisConnect(long handle);
	void CreateCheckThread();
	static UINT WINAPI CheckThreadProc(LPVOID pParam);
	void RunCheck();//��������ж��Ƿ������ӶϿ��߳�
};


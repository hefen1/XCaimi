/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DataSourceImpl.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	��������ӿ�ʵ��
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
#include "TradeComm/TradeCoreIntf.h"
#include "WndRecvData.h"
#include "DataSourceWorkThread.h"
#include "RequestResponceDataManager.h"
#include "stockManager.h"
#include "CommBourseManager.h"
#include "SubscriptionManager.h"
#include "DataSinkConnectManager.h"
#include "ReplyDataManger.h"
#include "AutoRequest.h"
#include "DSWriteFile.h"
#include "BlockManger.h"
#include "ConfigManager.h"
#include "ProgressDlg.h"
#include "DataCache.h"

#define	QS_INT			-1
#define	QS_UNINT		-2
#define	QS_LONG			-3
#define	QS_UNLONG		-4
#define	QS_SHORT		-5
#define	QS_WORD		    -6
#define	QS_UNSHORT		-6
#define	QS_FLOAT		-7
#define	QS_DOUBLE		-8

struct KQuickSort
{
	void*	m_pData;
	short	m_nDataLen;
	short	m_nKeyOffset;
	short	m_nKeyLen;
	long	m_nLow;
	long	m_nHigh;
};

struct Qentry{
	int left;
	int right;
};

class CDataSourceImpl : public IDataSource
{
protected:
	//���ݳ�ʼ����־
	BOOL								m_bInitFinished;
	//��������
	BOOL								m_bReConnect;
	
	//���׽ӿ� ���ڻ�ȡ�˺Ź�����
	ITradeCoreObj*	                    m_pTradeCore; 
	//��Ʊ����������
	CStockManager						m_stockManager;
	//�г�����������
	CHsBouseManager						m_bourseManager;
	//�����б����������
	CSubscriptionManager				m_subscriptionManager;
	//��������ע��ص�����Ӧ���ӹ�����
	CDataSinkConnectManager				m_datasinkconnectManager;
	//�Զ����������ʼ���ȣ�����
	CAutoRequest						m_autoRequest;
	//д�ļ�����
	CDSWriteFile						m_dsWriteFile;
	//ͳһ���ù�����
	CConfigManager						m_configManager;
	//��ʼ������������
	CProgressDlg*						m_pWndProgressDlg;
	//��һ�γ�ʼ����־
	BOOL								m_bFirstFlag;
	//���յ�RT_initainfo��
	BOOL								m_bInitInfo;
	BOOL								m_bIniting;
public:
	BOOL								m_bQuit;
	CWndRecvData*						m_pWndDataRecv;
	//
public:
	//-����ͻ�������
	BOOL								CtrolRequest(CDataSourceReqestResponceData* pData);
	//-���������Ӧ��
	BOOL								CtrolResponce(CDataSourceReqestResponceData* pData);
	//ͨѶ��ص��յ�����֪ͨ����������
	BOOL								OnReceivePacket(const char* pszBuffer,UINT uLen);
	//ͨѶ��������Ϣ֪ͨ
	BOOL								OnNetConnectNotify(ConnectNotify* pNotify);
	//���յ����ݣ����������ڲ�����
	BOOL								ProcessRecvData(CDataSourceReqestResponceData* pData);
	//���ػ������ݴ���
	BOOL								ProceLocalDeal(CDataSourceReqestResponceData* pData);
	//��ʼ��״̬�ص�����
	PFN_INITSTATUS_NOTIFY				m_pfnInitStatusNotify;
	CWnd*                               m_pNotifyWnd;
	//��ȡд�ļ�����ָ��
	CDSWriteFile*						GetDSWriterFile();
	//��ȡͳһ���ö���ָ��
	CConfigManager*						GetConfigManager();
	//��ȡ��Ʊ�������ָ��
	CStockManager*						GetStockManager();
	//��ȡ�г��������ָ��
	CHsBouseManager*					GetBouseManager();
	//���պͷ��Ͱ�ȫ�����͵��ض���lSink
	BOOL								SendSpecialSink(const char* pBuffer, long lDatalen, long lSink=-1);
protected:
	//�ҵ����������Ļص�sink�������ݷ��͵�sink
	BOOL								SendDataToSinks(UINT uAskType, CodeInfo* pInfo, char* pszDatabuff, int nBufferLen,UINT uStatus=-1);		
	//�ͷ����д��������Ķ���
	BOOL								FreeAllUnit();
	//��ʼ���������沿�ֱ���
	BOOL								InitEnv();
	//�����г�����Ʊ�����ʼ��
	BOOL								InitMarketStocksInfo();
	//
	long                                  DecideConnectByAskData(AskData* pAsk);
protected:
	//���յ������ݾ��崦��
	int								DataProcessing(PacketInfo* pRecv, CReplyDataManger& manager);
	int								DataProcess_Init(PacketInfo* pRecv, CReplyDataManger& manager);
	int								DataProcess_Login(PacketInfo* pRecv, CReplyDataManger& manager);
	int								DataProcess_RealTimeExt(PacketInfo* pRecv, CReplyDataManger& manager);
	int								DataProcess_RealTime(PacketInfo* pRecv, CReplyDataManger& manager);
	int								DataProcess_ReplyToSink(HWND hwndAsync, CReplyDataManger& manager);
	int								DataProcess_ServerCalc(PacketInfo* pRecv, CReplyDataManger& manager);
	int								DataProcess_TechDataEx(PacketInfo* pRecv, CReplyDataManger& manager);
	int								DataProcess_CacheDataEx(PacketInfo* pRecv, CReplyDataManger& manager);
	int								DataProcess_ServerTimeEx(PacketInfo* pRecv, CReplyDataManger& manager);
	int								DataProcess_UnDeal(PacketInfo* pRecv, CReplyDataManger& manager);
	void                          DataProcess_DiskDataEx(PacketInfo* pRecv);

public:
//construction
	CDataSourceImpl();
	~CDataSourceImpl();
protected:
	//������ʱ��ĶԱ�ֵ
	int						m_nServerDateSpan;
	int						m_nServerTimeSpan;
	long					m_lServerTime;
	long					m_lCurTime;
	//ͨѶ����
//	HINSTANCE				m_hInstConnect;
	IChannelManager*		m_pChannelManager;
	IChannels*				    m_pChannels;
	//long					        m_lConnectHandle;  
	long					        m_lNotifySink;             //��Ҫȫ������֪ͨ��sink
	CDataCache              m_DataCache; //�������
public:
	//���߳����ݴ������ͬ�������߳�
	LRESULT					OnAsyncNotify(WPARAM wParam, LPARAM lParam);
	//״̬֪ͨ�����߳�
	LRESULT					OnStatusNotify(WPARAM wParam, LPARAM lParam);

//ͨѶ�����ݻص�����
	OnPacketReceive		m_pfnOnPacketReceive;
	OnConnectNotify		m_pfnOnConnectNotify;
//interface
public:
	virtual long	HSDataSource_DataSouceInit(IDataSourceSink* pSink, const char* pszProfile = NULL) ;
	virtual BOOL	HSDataSource_DataSourceUnInit(long lDataSourceID);
	virtual BOOL	HSDataSource_RequestAsyncData(long lDataSourceID, char* pAskData, int nAskLen, int nTimeOut, 
						UINT uSendOptions = e_DataSouceSend_Normal, long lConnectHandle = 0, long lPackID = 0);
	virtual void*	HSDataSource_RequestSyncData(long lDataSourceID, char* pAskData, int nAskLen, int nTimeOut, 
						int& pLen, UINT uSendOptions = e_DataSouceSend_Normal, long lConnectHandle = 0, long lPackID = 0);
	virtual BOOL	HSDataSource_SyncDataRefComplete(long lDataSourceID, void* pRefData, int nRefDataLen, long lConnectHandle = 0);
	virtual CString	HSDataSource_GetLastError(long lConnectHandle = 0);
	virtual long	HSDataSource_ServerDataInit(long lConnectHandle, PFN_INITSTATUS_NOTIFY fn_Notify, CWnd* pWnd);
	virtual void    HSDataSource_SetDefaultConnectHandle(IN long lConnectHandle);
	virtual  BOOL	HSDataSource_ReConnectNotify(IN long lConnectHandle);
	virtual BOOL	HSDataSourceEx_GetStockUserInfo(CodeInfo* pInfo, StockUserInfo& stockInfo);
	virtual BOOL	HSDataSourceEx_GetServerDataTime(long &lDate, long &lTime);
	virtual int		HSDataSourceEx_GetMarketStocksInfo(unsigned short nMarket, CArray<StockUserInfo>& stockArray);
	virtual int		HSDataSourceEx_GetSystemMarketInfo(unsigned short* pMarket, int nMarketCount, CArray<StockType>& blockArray);
	virtual BOOL	HSDataSourceEx_GetStockUserInfo(char* pszCode, unsigned short nMarket, StockUserInfo& stockInfo, unsigned short nMarketException = 0);
	virtual int		HSDataSourceEx_GetStocksUserInfo(IN CArray<CodeInfo>& arInfo, IN OUT CArray<StockUserInfo>& stockInfo, BOOL bCompareOnlyMainMarket = FALSE);
	virtual int		HSDataSourceEx_GetStocksFinanceData(IN CArray<CodeInfo>& arInfo, IN OUT CArray<CurrentFinanceData>& ayFinanceData);
	virtual int		HSDataSourceEx_GetStocksExrightData(IN CArray<CodeInfo>& arInfo, IN OUT CArray<CurrentExRightData>& ayExrightData);
	virtual void*	HSDataSource_GetDayData(IN void* pAsk, IN int nMask = 0,IN int nExRight = 0) ;
	virtual BOOL    HSDataSource_WriteDayData(CodeInfo* info,int period,StockCompDayDataEx* pNewData,int nSize );
	virtual int     HSDataSource_FindStockFromKey(StockUserInfo* pStock, const CString& strKey,int& nUnique,
										int& nStart,int& nEnd, CString &szMarch,int& nBetter,const int nCurIndex) ;
	virtual CString HSDataSource_GetStockPyjc(StockUserInfo* pStock) ;
};


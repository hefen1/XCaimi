/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	DataSourceImpl.h
*	文件标识：
*	摘	  要：	数据引擎接口实现
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-7-13
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
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
	//数据初始化标志
	BOOL								m_bInitFinished;
	//断线重连
	BOOL								m_bReConnect;
	
	//交易接口 用于获取账号管理器
	ITradeCoreObj*	                    m_pTradeCore; 
	//股票管理器对象
	CStockManager						m_stockManager;
	//市场管理器对象
	CHsBouseManager						m_bourseManager;
	//订阅列表管理器对象
	CSubscriptionManager				m_subscriptionManager;
	//数据引擎注册回调及对应连接管理器
	CDataSinkConnectManager				m_datasinkconnectManager;
	//自动请求（例如初始化等）对象
	CAutoRequest						m_autoRequest;
	//写文件对象
	CDSWriteFile						m_dsWriteFile;
	//统一配置管理器
	CConfigManager						m_configManager;
	//初始化进度条窗口
	CProgressDlg*						m_pWndProgressDlg;
	//第一次初始化标志
	BOOL								m_bFirstFlag;
	//已收到RT_initainfo包
	BOOL								m_bInitInfo;
	BOOL								m_bIniting;
public:
	BOOL								m_bQuit;
	CWndRecvData*						m_pWndDataRecv;
	//
public:
	//-处理客户端请求
	BOOL								CtrolRequest(CDataSourceReqestResponceData* pData);
	//-处理服务器应答
	BOOL								CtrolResponce(CDataSourceReqestResponceData* pData);
	//通讯层回调收到数据通知到数据引擎
	BOOL								OnReceivePacket(const char* pszBuffer,UINT uLen);
	//通讯层连接消息通知
	BOOL								OnNetConnectNotify(ConnectNotify* pNotify);
	//接收到数据，数据引擎内部处理
	BOOL								ProcessRecvData(CDataSourceReqestResponceData* pData);
	//本地缓存数据处理
	BOOL								ProceLocalDeal(CDataSourceReqestResponceData* pData);
	//初始化状态回调函数
	PFN_INITSTATUS_NOTIFY				m_pfnInitStatusNotify;
	CWnd*                               m_pNotifyWnd;
	//获取写文件对象指针
	CDSWriteFile*						GetDSWriterFile();
	//获取统一配置对象指针
	CConfigManager*						GetConfigManager();
	//获取股票管理对象指针
	CStockManager*						GetStockManager();
	//获取市场管理对象指针
	CHsBouseManager*					GetBouseManager();
	//接收和发送包全部发送到特定的lSink
	BOOL								SendSpecialSink(const char* pBuffer, long lDatalen, long lSink=-1);
protected:
	//找到符合条件的回调sink，把数据发送到sink
	BOOL								SendDataToSinks(UINT uAskType, CodeInfo* pInfo, char* pszDatabuff, int nBufferLen,UINT uStatus=-1);		
	//释放所有创建出来的对象
	BOOL								FreeAllUnit();
	//初始化数据引擎部分变量
	BOOL								InitEnv();
	//本地市场及股票代码初始化
	BOOL								InitMarketStocksInfo();
	//
	long                                  DecideConnectByAskData(AskData* pAsk);
protected:
	//接收到的数据具体处理
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
	//服务器时间的对比值
	int						m_nServerDateSpan;
	int						m_nServerTimeSpan;
	long					m_lServerTime;
	long					m_lCurTime;
	//通讯对象
//	HINSTANCE				m_hInstConnect;
	IChannelManager*		m_pChannelManager;
	IChannels*				    m_pChannels;
	//long					        m_lConnectHandle;  
	long					        m_lNotifySink;             //需要全部包都通知的sink
	CDataCache              m_DataCache; //缓存变量
public:
	//多线程数据处理完毕同步到主线程
	LRESULT					OnAsyncNotify(WPARAM wParam, LPARAM lParam);
	//状态通知到主线程
	LRESULT					OnStatusNotify(WPARAM wParam, LPARAM lParam);

//通讯层数据回调函数
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


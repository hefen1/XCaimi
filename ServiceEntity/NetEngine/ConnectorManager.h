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
	* 函数功能 :创建指定连接
	* 函数参数 :Server_type cType: 连接类型
	*           CString strName: 服务器名称(空表示默认站点)
	*           成功返回Connector接口指针，反之为NULL
	*  返回值  :成功返回0，反之为FALSE	
	*******************************************************************************/
	virtual long CreateConnector(Server_type cType,CString strName,TradeCert *ptr = NULL) ;

	/********************************************************************************
	* 函数功能 :创建最优连接
	* 函数参数 :Server_type cType: 连接类型
	*  返回值  :成功返回Connector接口指针，反之为NULL	
	*******************************************************************************/
	virtual long CreateBestConnector(Server_type cType,TradeCert *ptr = NULL);


	/********************************************************************************
	* 函数功能 :创建最优连接
	* 函数参数 :Connector *pConnect: Creator返回的连接接口
	*  返回值  :成功返回0
	*******************************************************************************/
	virtual int DestroyConnect(long handle);

	/********************************************************************************
	* 函数功能 :测速
	* 函数参数 :Server_type cType: 所要测速的服务器类型
	* 函数参数 :TestSpeedCallBack tsCall:测速回调，以返回测速信息
	*  返回值  :成功返回0
	* 在调用该函数前必须先调用SetOnlineRadioRange和SetHandleTimeRange设置权重
	*******************************************************************************/
	virtual int TestSiteSpeed(Server_type cType,TestSpeedCallBack tsCall,void *ptr);

	/********************************************************************************
	* 函数功能 :停止测速
	*  返回值  :成功返回0
	*******************************************************************************/
	virtual int StopSiteTest(Server_type cType);


	/********************************************************************************
	* 函数功能 :注册通知回调
	* 函数参数 :OnConnectNotify notify:通知回调函数
	*  返回值  :成功返回0
	*******************************************************************************/
	virtual int RegisterNotify(OnConnectNotify notify);


	/********************************************************************************
	* 函数功能 :取消注册通知回调
	* 函数参数 :OnConnectNotify notify:通知回调函数
	*  返回值  :成功返回0
	*******************************************************************************/
	virtual int UnregisterNotify(OnConnectNotify notify);
	/********************************************************************************
	* 函数功能 :设置测速结果计算比重范围
	* 函数参数 :Server_type cType: 服务器类型
	* 函数参数 :RangeItem数组指针
	* 函数参数 :RangeItem数组元素个数
	*******************************************************************************/
	virtual void SetOnlineRadioRange(Server_type cType,RangeItem *pOnline,int count);
	virtual void SetHandleTimeRange(Server_type cType,RangeItem *pTime,int count);
	/********************************************************************************
	* 函数功能 :设置服务器信息
	* 函数参数 :CommInfo数组指针
	* 函数参数 :数组元素个数
	* 函数参数 :ProxyInfo结构指针
	* 函数参数：type要设置的服务器类型，默认是所有服务器
	*********************************************************************************/
	virtual int SetSites(CommInfo *pComm, int count,ProxyInfo *pProxy,Server_type type = CEV_Connect_ALL);
	/********************************************************************************
	* 函数功能 :测试代理服务器
	* 函数参数 :ProxyInfo结构指针
	* 返回值：成功返回0，否则-1
	********************************************************************************/
	virtual int TestProxy(ProxyInfo *pProxy);
	/********************************************************************************
	* 函数功能 :根据句柄获取站点名称
	* 函数参数 :连接句柄
	* 返回值：返回站点名称
	********************************************************************************/
	virtual CString GetServerName(long handle);

	/********************************************************************************
	* 函数功能 :根据句柄获取MAC地址
	* 函数参数 :连接句柄
	* 返回值：返回MAC地址
	********************************************************************************/
	virtual CString GetMacAdd(long handle);
	/********************************************************************************
	* 函数功能 :连接下一个站点
	* 函数参数 : 前一个站点的站点名
	* 返回值：连接句柄
	********************************************************************************/
	virtual long ConnectNextSite(Server_type cType,CString strName,TradeCert *ptr);

public:
	bool IsConnected(long handle);
	BOOL IsConnectHandle(long handle);
	float GetOnlineRadioRange(Server_type cType,float fValue);
	float GetHandleTimeRange(Server_type cType,float fValue);
	BOOL  Notify(ConnectNotify *pNot);
	static BOOL IsHQServerType(Server_type type);  //是否是行情类连接
	BOOL FConnNotify(ConnectNotify *pnot);

protected:
	void DestroyRemovedConn();//通讯设置后有可能将当前连接删掉了,移除已删除的连接
	void Free();

protected:
	ServerInfo    m_siteInfoList;      //所有站点信息列表
	CArray<OnConnectNotify,OnConnectNotify> m_notifyList; //连接状态通知回调列表
	CArray<long,long> m_connectedSiteList;   //当前连接信息列表
	CArray<long,long> m_disConnectList;     //断开的连接列表
	CCriticalSection m_cCritical_DisConn;
	CCriticalSection m_cCritical_Conn;       //临界区对象
	CCriticalSection m_cCritical_Notify;

protected:
	CMapWordToPtr m_mapSpeedTest; //保存测速对象，每个服务器类型一个CServerSpeedTest类实例
	CMapWordToPtr m_speedTestRange;//保存测速计算用到的比率

protected:
	CMap<long,long,void*,void*> m_mapHeartBeat;//key:连接handle，value:心跳对象
	CCriticalSection m_cCritical_Heart;       //临界区对象
	CArray<ConnectNotify*,ConnectNotify*> m_ayConnNotify;//连接状态通知
	CCriticalSection m_cCritical_ConnNotify;       //临界区对象
	HANDLE m_hEventConnNotify;
	HANDLE m_hThreadCheck;
	void CreateHeartBeat(long handle);
	void StopHeartBeat(long handle);
	void DeleteConnect(long handle);
	void RemoveDisConnect(long handle);
	void CreateCheckThread();
	static UINT WINAPI CheckThreadProc(LPVOID pParam);
	void RunCheck();//检查心跳判断是否有连接断开线程
};


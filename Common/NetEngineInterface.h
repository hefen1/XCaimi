#pragma once
#pragma	pack(1)

#define LOGIN_KEY                2
#define LEVEL2LOGIN_KEY          3
enum Server_type//服务器类型
{
	CEV_Connect_ALL = 0,
	CEV_Connect_HQ=1,
	CEV_Connect_QH,
	CEV_Connect_LEVEL2,
	CEV_Connect_JY,
	CEV_Connect_ZX,
	CEV_Connect_CENTER
};
enum Proxy_type//代理类型
{
	NoProxy=-1,
	Socks4=0,
	Http,
	Socks5
};
enum HJC_EI_type//交易连接类型
{
	TCP=1,
	SSL,
	TCP_7611,
	TCP_7612,
	SSL_7611,
	SSL_7612
};
enum ConnectResult//连接结果
{
	Linked=1,
	Drop,
	UnLogin
};

//客户端接受数据通用包
struct PacketInfo
{
	long m_lHandle;			//连接handle
	long m_lDataTotal;			//数据长度
	long m_lDataTransmited;		//已经传送的数据长度
	const char* m_pszData;		// if m_bFile is TRUE, m_pszData points to file name
};

struct CommInfo    //服务器信息
{
	Server_type   m_cType;        //服务器类型
	char    m_cIsDefault;     //是否是默认服务器
	char	    m_strServerID[128];		//服务器名称
	char	    m_strServerIP [128];	   // IP
	unsigned short  m_nPort;		  // port
};

struct ProxyInfo   //代理信息
{
	char	    m_strUser[64];					//	用户名
	char    	m_strPwd[64];					//	用户密码
	char 	m_strProxyIP[128];				//	代理服务器地址
	int		m_nPort;						//	代理服务器端口
	Proxy_type	 m_nOptions;			    //	代理协议
	int     m_nUseChannel;					// 使用绿色通道
};

struct RangeItem//测速结果计算用的
{
	float fBegin;
	float fEnd;
	float fBets;
};

struct TradeCert//交易连接参数结构
{
	HJC_EI_type m_Type;
	char m_ca[256];
	char m_cert[256];
	char m_key[256];
	char m_pwd[256];
};

struct SpeedTestResult
{
	char	    m_strServerID[128];		//服务器名称
	float m_onlineRadio;
	DWORD m_timeSpan;
	DWORD m_result;
	BOOL m_bError; //是否出错
	char m_strError[256];//错误信息
};

struct ConnectNotify
{
	long m_lHandle;
	ConnectResult m_result;
	Server_type m_cType;
};

typedef BOOL(WINAPI *OnPacketReceive)(const char*,UINT); 
typedef BOOL(WINAPI *TestSpeedCallBack)(SpeedTestResult*); 
typedef BOOL(WINAPI *OnConnectNotify)(ConnectNotify*); 

#define OBJ_CHANNELS   _T("NetEngine.Channels")
#define OBJ_CHANNELMANAGER   _T("NetEngine.ChannelManager")
struct IChannels
{
	/********************************************************************************
	* 函数功能 :同步发送接收数据包	
	* 函数参数 :long handle:连接句柄
	*           const char* ReqBuf: 发送数据包指针
	*           UINT nReqLen:  发送的数据包长度
	*           dword nTimeOut: 超时时间
	*           const char* RcvBuf: 接收数据包指针（用于保存数据）
	*           UINT nRcvLen:  接收的数据包长度
	*  返回值  :成功返回0，反之为-1	
	*******************************************************************************/ 
	virtual int SendRcv(long handle,const char* ReqBuf,UINT nReqLen,DWORD nTimeOut,char*& RcvBuf,UINT& nRcvLen) = 0;
	virtual int FreeMemory(long handle,char *pMem) = 0;
	/********************************************************************************
	* 函数功能 :异步发送数据包,应答通过RegisterProcessor注册的处理函数自行处理	
	* 函数参数 :const char* szBuf: 发送数据包指针
	*           UINT nBufLen:  发送的数据包长度
	*			DWORD TimeOut: 超时时间-1不超时
	*  返回值  :成功返回0，反之为-1
	*******************************************************************************/
	virtual int AsyncSend(long handle,const char* szBuf,UINT nBufLen,DWORD TimeOut = -1) = 0;


	/********************************************************************************
	* 函数功能 :注册异步应答包处理函数
	* 函数参数 :long handle:连接句柄
	*           OnPacketReceive processor: 处理函数指针
	*  返回值  :成功返回0，反之为FALSE	
	*******************************************************************************/
	virtual int RegisterProcessor(long handle,OnPacketReceive processor) = 0;
	virtual int UnRegisterProcessor(long handle,OnPacketReceive processor) = 0;
	/********************************************************************************
	* 函数功能 :取最近的错误信息
	* 函数参数 :long handle:连接句柄
	*  返回值  :通号handleh上的错误信息
	*******************************************************************************/
	virtual CString GetLastEorror(long handle)=0;
	virtual UINT GetPacketID(long handle)=0;

	virtual Server_type GetServerTypeByHandle(long handle) = 0;
};

struct IChannelManager 
{
	/********************************************************************************
	* 函数功能 :创建指定连接
	* 函数参数 :Server_type cType: 连接类型
	*           CString strName: 服务器名称(空表示默认站点)
	*           成功返回Connector接口指针，反之为NULL
	*  返回值  :成功返回0，反之为FALSE	
	*******************************************************************************/
	virtual long CreateConnector(Server_type cType,CString strName,TradeCert* ptr = NULL) = 0;

	/********************************************************************************
	* 函数功能 :创建最优连接
	* 函数参数 :Server_type cType: 连接类型
	*  返回值  :成功返回Connector接口指针，反之为NULL	
	*******************************************************************************/
	virtual long CreateBestConnector(Server_type cType,TradeCert* ptr = NULL) = 0;


	/********************************************************************************
	* 函数功能 :创建最优连接
	* 函数参数 :Connector *pConnect: Creator返回的连接接口
	*  返回值  :成功返回0
	*******************************************************************************/
	virtual int DestroyConnect(long handle) = 0;

	/********************************************************************************
	* 函数功能 :创建最优连接
	* 函数参数 :Server_type cType: 所要测速的服务器类型
	* 函数参数 :TestSpeedCallBack tsCall:测速回调，以返回测速信息
	*  返回值  :成功返回0
	* 在调用该函数前必须先调用SetOnlineRadioRange和SetHandleTimeRange设置权重
	*******************************************************************************/
	virtual int TestSiteSpeed(Server_type cType,TestSpeedCallBack tsCall, void *ptr) = 0;

	/********************************************************************************
	* 函数功能 :停止测速
	*  返回值  :成功返回0
	*******************************************************************************/
	virtual int StopSiteTest(Server_type cType) = 0;


	/********************************************************************************
	* 函数功能 :注册通知回调
	* 函数参数 :OnConnectNotify notify:通知回调函数
	*  返回值  :成功返回0
	*******************************************************************************/
	virtual int RegisterNotify(OnConnectNotify notify) = 0;


	/********************************************************************************
	* 函数功能 :取消注册通知回调
	* 函数参数 :OnConnectNotify notify:通知回调函数
	*  返回值  :成功返回0
	*******************************************************************************/
	virtual int UnregisterNotify(OnConnectNotify notify) = 0;
	/********************************************************************************
	* 函数功能 :设置测速结果计算比重范围
	* 函数参数 :Server_type cType: 服务器类型
	* 函数参数 :RangeItem数组指针
	* 函数参数 :RangeItem数组元素个数
	*******************************************************************************/
	virtual void SetOnlineRadioRange(Server_type cType,RangeItem *pOnline,int count) = 0;
	virtual void SetHandleTimeRange(Server_type cType,RangeItem *pTime,int count) = 0;
	/********************************************************************************
	* 函数功能 :设置服务器信息
	* 函数参数 :CommInfo数组指针
	* 函数参数 :数组元素个数
	* 函数参数 :ProxyInfo结构指针
	* 函数参数：type要设置的服务器类型，默认是所有服务器
	*********************************************************************************/
	virtual int SetSites(CommInfo *pComm, int count,ProxyInfo *pProxy,Server_type type = CEV_Connect_ALL) = 0;
	/********************************************************************************
	* 函数功能 :测试代理服务器
	* 函数参数 :ProxyInfo结构指针
	* 返回值：成功返回0，否则-1
	********************************************************************************/
	virtual int TestProxy(ProxyInfo *pProxy) = 0;	
	/********************************************************************************
	* 函数功能 :根据句柄获取站点名称
	* 函数参数 :连接句柄
	* 返回值：返回站点名称
	********************************************************************************/
	virtual CString GetServerName(long handle) = 0;

	/********************************************************************************
	* 函数功能 :根据句柄获取MAC地址
	* 函数参数 :连接句柄
	* 返回值：返回MAC地址
	********************************************************************************/
	virtual CString GetMacAdd(long handle) = 0;
};
#pragma	pack()

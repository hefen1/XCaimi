#pragma once
#pragma	pack(1)

#define LOGIN_KEY                2
#define LEVEL2LOGIN_KEY          3
enum Server_type//����������
{
	CEV_Connect_ALL = 0,
	CEV_Connect_HQ=1,
	CEV_Connect_QH,
	CEV_Connect_LEVEL2,
	CEV_Connect_JY,
	CEV_Connect_ZX,
	CEV_Connect_CENTER
};
enum Proxy_type//��������
{
	NoProxy=-1,
	Socks4=0,
	Http,
	Socks5
};
enum HJC_EI_type//������������
{
	TCP=1,
	SSL,
	TCP_7611,
	TCP_7612,
	SSL_7611,
	SSL_7612
};
enum ConnectResult//���ӽ��
{
	Linked=1,
	Drop,
	UnLogin
};

//�ͻ��˽�������ͨ�ð�
struct PacketInfo
{
	long m_lHandle;			//����handle
	long m_lDataTotal;			//���ݳ���
	long m_lDataTransmited;		//�Ѿ����͵����ݳ���
	const char* m_pszData;		// if m_bFile is TRUE, m_pszData points to file name
};

struct CommInfo    //��������Ϣ
{
	Server_type   m_cType;        //����������
	char    m_cIsDefault;     //�Ƿ���Ĭ�Ϸ�����
	char	    m_strServerID[128];		//����������
	char	    m_strServerIP [128];	   // IP
	unsigned short  m_nPort;		  // port
};

struct ProxyInfo   //������Ϣ
{
	char	    m_strUser[64];					//	�û���
	char    	m_strPwd[64];					//	�û�����
	char 	m_strProxyIP[128];				//	�����������ַ
	int		m_nPort;						//	����������˿�
	Proxy_type	 m_nOptions;			    //	����Э��
	int     m_nUseChannel;					// ʹ����ɫͨ��
};

struct RangeItem//���ٽ�������õ�
{
	float fBegin;
	float fEnd;
	float fBets;
};

struct TradeCert//�������Ӳ����ṹ
{
	HJC_EI_type m_Type;
	char m_ca[256];
	char m_cert[256];
	char m_key[256];
	char m_pwd[256];
};

struct SpeedTestResult
{
	char	    m_strServerID[128];		//����������
	float m_onlineRadio;
	DWORD m_timeSpan;
	DWORD m_result;
	BOOL m_bError; //�Ƿ����
	char m_strError[256];//������Ϣ
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
	* �������� :ͬ�����ͽ������ݰ�	
	* �������� :long handle:���Ӿ��
	*           const char* ReqBuf: �������ݰ�ָ��
	*           UINT nReqLen:  ���͵����ݰ�����
	*           dword nTimeOut: ��ʱʱ��
	*           const char* RcvBuf: �������ݰ�ָ�루���ڱ������ݣ�
	*           UINT nRcvLen:  ���յ����ݰ�����
	*  ����ֵ  :�ɹ�����0����֮Ϊ-1	
	*******************************************************************************/ 
	virtual int SendRcv(long handle,const char* ReqBuf,UINT nReqLen,DWORD nTimeOut,char*& RcvBuf,UINT& nRcvLen) = 0;
	virtual int FreeMemory(long handle,char *pMem) = 0;
	/********************************************************************************
	* �������� :�첽�������ݰ�,Ӧ��ͨ��RegisterProcessorע��Ĵ��������д���	
	* �������� :const char* szBuf: �������ݰ�ָ��
	*           UINT nBufLen:  ���͵����ݰ�����
	*			DWORD TimeOut: ��ʱʱ��-1����ʱ
	*  ����ֵ  :�ɹ�����0����֮Ϊ-1
	*******************************************************************************/
	virtual int AsyncSend(long handle,const char* szBuf,UINT nBufLen,DWORD TimeOut = -1) = 0;


	/********************************************************************************
	* �������� :ע���첽Ӧ���������
	* �������� :long handle:���Ӿ��
	*           OnPacketReceive processor: ������ָ��
	*  ����ֵ  :�ɹ�����0����֮ΪFALSE	
	*******************************************************************************/
	virtual int RegisterProcessor(long handle,OnPacketReceive processor) = 0;
	virtual int UnRegisterProcessor(long handle,OnPacketReceive processor) = 0;
	/********************************************************************************
	* �������� :ȡ����Ĵ�����Ϣ
	* �������� :long handle:���Ӿ��
	*  ����ֵ  :ͨ��handleh�ϵĴ�����Ϣ
	*******************************************************************************/
	virtual CString GetLastEorror(long handle)=0;
	virtual UINT GetPacketID(long handle)=0;

	virtual Server_type GetServerTypeByHandle(long handle) = 0;
};

struct IChannelManager 
{
	/********************************************************************************
	* �������� :����ָ������
	* �������� :Server_type cType: ��������
	*           CString strName: ����������(�ձ�ʾĬ��վ��)
	*           �ɹ�����Connector�ӿ�ָ�룬��֮ΪNULL
	*  ����ֵ  :�ɹ�����0����֮ΪFALSE	
	*******************************************************************************/
	virtual long CreateConnector(Server_type cType,CString strName,TradeCert* ptr = NULL) = 0;

	/********************************************************************************
	* �������� :������������
	* �������� :Server_type cType: ��������
	*  ����ֵ  :�ɹ�����Connector�ӿ�ָ�룬��֮ΪNULL	
	*******************************************************************************/
	virtual long CreateBestConnector(Server_type cType,TradeCert* ptr = NULL) = 0;


	/********************************************************************************
	* �������� :������������
	* �������� :Connector *pConnect: Creator���ص����ӽӿ�
	*  ����ֵ  :�ɹ�����0
	*******************************************************************************/
	virtual int DestroyConnect(long handle) = 0;

	/********************************************************************************
	* �������� :������������
	* �������� :Server_type cType: ��Ҫ���ٵķ���������
	* �������� :TestSpeedCallBack tsCall:���ٻص����Է��ز�����Ϣ
	*  ����ֵ  :�ɹ�����0
	* �ڵ��øú���ǰ�����ȵ���SetOnlineRadioRange��SetHandleTimeRange����Ȩ��
	*******************************************************************************/
	virtual int TestSiteSpeed(Server_type cType,TestSpeedCallBack tsCall, void *ptr) = 0;

	/********************************************************************************
	* �������� :ֹͣ����
	*  ����ֵ  :�ɹ�����0
	*******************************************************************************/
	virtual int StopSiteTest(Server_type cType) = 0;


	/********************************************************************************
	* �������� :ע��֪ͨ�ص�
	* �������� :OnConnectNotify notify:֪ͨ�ص�����
	*  ����ֵ  :�ɹ�����0
	*******************************************************************************/
	virtual int RegisterNotify(OnConnectNotify notify) = 0;


	/********************************************************************************
	* �������� :ȡ��ע��֪ͨ�ص�
	* �������� :OnConnectNotify notify:֪ͨ�ص�����
	*  ����ֵ  :�ɹ�����0
	*******************************************************************************/
	virtual int UnregisterNotify(OnConnectNotify notify) = 0;
	/********************************************************************************
	* �������� :���ò��ٽ��������ط�Χ
	* �������� :Server_type cType: ����������
	* �������� :RangeItem����ָ��
	* �������� :RangeItem����Ԫ�ظ���
	*******************************************************************************/
	virtual void SetOnlineRadioRange(Server_type cType,RangeItem *pOnline,int count) = 0;
	virtual void SetHandleTimeRange(Server_type cType,RangeItem *pTime,int count) = 0;
	/********************************************************************************
	* �������� :���÷�������Ϣ
	* �������� :CommInfo����ָ��
	* �������� :����Ԫ�ظ���
	* �������� :ProxyInfo�ṹָ��
	* ����������typeҪ���õķ��������ͣ�Ĭ�������з�����
	*********************************************************************************/
	virtual int SetSites(CommInfo *pComm, int count,ProxyInfo *pProxy,Server_type type = CEV_Connect_ALL) = 0;
	/********************************************************************************
	* �������� :���Դ��������
	* �������� :ProxyInfo�ṹָ��
	* ����ֵ���ɹ�����0������-1
	********************************************************************************/
	virtual int TestProxy(ProxyInfo *pProxy) = 0;	
	/********************************************************************************
	* �������� :���ݾ����ȡվ������
	* �������� :���Ӿ��
	* ����ֵ������վ������
	********************************************************************************/
	virtual CString GetServerName(long handle) = 0;

	/********************************************************************************
	* �������� :���ݾ����ȡMAC��ַ
	* �������� :���Ӿ��
	* ����ֵ������MAC��ַ
	********************************************************************************/
	virtual CString GetMacAdd(long handle) = 0;
};
#pragma	pack()

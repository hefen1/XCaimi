/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	DataSinkConnectManager.h
*	文件标识：
*	摘	  要：	数据引擎注册回调及对应连接管理器
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-9-1
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma  once
struct CDataSourceSinkToConnectID
{
	long		m_lConnectHandle;				//通讯连接句柄
	long		m_lDataSinkID;					//回调
	long		m_lPackID;						//包序号
	UINT		m_uSinkType;					//调用类型 - 交易，行情

	CDataSourceSinkToConnectID()
	{
		memset(this, 0, sizeof(CDataSourceSinkToConnectID));
	}
};

class CDataSinkConnectManager
{
protected:
	//本地缓存对应关系队列
	std::list<CDataSourceSinkToConnectID*>	m_listData;
	//缓存对应关系互斥量
	CMutex									m_mutex;	

public:
	//	构造函数与析构函数
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
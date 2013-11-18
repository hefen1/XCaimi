/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	SubscriptionManager.h
*	文件标识：
*	摘	  要：	订阅列表管理类
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
#pragma once
#include "RequestResponceDataManager.h"

struct CDataSourceSubscription
{
	UINT					m_uAskType;					//请求功能号
	DWORD					m_dwPacketID;               //请求包id
	CodeInfo				m_codeInfo;					//证券信息
	std::list<long>			m_listDataSinkID;			//外部调用模块所对应的ID列表
	DWORD					m_dwTickRequest;			//上次有效请求时所记录的时序（主推数据该字段无效）
	DWORD					m_dwTickResponce;			//上次应答所对应的时序（主推数据该字段无效）
	UINT                    m_uResever;					//保留 盘后分析包 存放周期类型
};

//typedef CMap<CodeInfo*, CodeInfo*, BOOL, BOOL>CMapPreSubscrition;

enum eSubscriptionStatus		//订阅列表状态
{
	eSubscriptionStatus_unknow		= 0,			//未知订阅
	eSubscriptionStatus_new			= 0x0001,		//新的订阅
	eSubscriptionStatus_add			= 0x0010,		//老订阅，增加回调入口
	eSubscriptionStatus_exist		= 0x0020,		//已经存在，不处理
	eSubscriptionStatus_remove		= 0x0100,		//删除订阅

	eSubscriptionStatus_maskAdd		= 0x000F,		//掩码-需要新请求
	eSubscriptionStatus_maskExist	= 0x00F0,		//掩码-不需要新请求
	eSubscriptionStatus_maskDel		= 0x0F00,		//掩码-需要删除原请求
};

#define HS_SUBSCRIPTION_IGNORE_INTERVAL			1000				//订阅忽略间隔时间 - 3秒

class CSubscriptionManager
{
protected:
	//订阅列表同步信号量
	CRITICAL_SECTION					m_critical;			
	//订阅列表队列
	std::list<CDataSourceSubscription*>	m_listSubscription;
public:
	//	构造函数与析构函数
	CSubscriptionManager();
	~CSubscriptionManager();
protected:
	//查询订阅状况及更新
//	int									CheckSubscriptionInfo(UINT uAskType, CodeInfo* pCodeInfo, long lDataSinkID);
	BOOL								FindCodeInfo(CodeInfo* pSource, CodeInfo* pCodeArray, int nArSize);
public:
	//清空本地订阅数据
	BOOL								RemoveAllSubscriptions();
	//清空指定本地订阅数据
	BOOL								RemoveSubscriptions(UINT uAskType, CodeInfo* pCodeInfo, UINT uPacketID = 0);
	//获取原订阅
	CDataSourceSubscription*			GetSubscription(UINT uAskType, CodeInfo* pCodeInfo, UINT uPacketID = 0);
	
	//清空指定的订阅
//	int									ClearLastSubscription(UINT uAskType, long lDataSinkID);
	//根据请求类型，代码，回调入口参数等，组合成请求包
	
	/********************************************************************************
	* 函数功能 :根据未处理的请求包，获取一个到服务器的请求包
	* 函数参数 :CDataSourceReqestResponceData* pData - 未处理的请求包
	*			int& nAskCount - 记录条数，引用
	*  返回值  :到服务器的请求包	
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	char*								GetRequestPack(IN CDataSourceReqestResponceData* pData, IN OUT int& nAskCount);

	//某个sink被移除
	BOOL								RemoveSink(long lSinkID);
	//增加某个请求的订阅信息
	BOOL								UpdateOneSubscription(UINT uAskType, long lDataSinkID, AskData* pAsk = NULL);
};


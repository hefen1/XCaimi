/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	DataSourceDefine.h
*	文件标识：
*	摘	  要：	数据引擎接口定义头文件
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

#include "DataSourceSinkDefine.h"

enum{
	e_DataSouceSend_HQData = 0,			//行情数据
	e_DataSouceSend_JYData = 0x01,		//交易数据
	
	e_DataSouceSend_High = 0x010,		//高优先级
	e_DataSouceSend_Normal = 0x020,
	e_DataSouceSend_Low = 0x040,

	e_DataSouceSend_Add = 0x0100,		//普通增加的操作
	e_DataSouceSend_Clear = 0x0200,		//清空包

	e_DataSouceSend_ReadDisk = 0x1000,	//读本地

	e_DataSourceSend_HQDataDownLoad = 0x2000,  //行情数据下载
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
	* 函数功能 :设置数据引擎回调入口
	* 函数参数 :IDataSourceSink* pSink - 回调的句柄
	*			 const char* pszProfile - 
	*  返回值  :数据引擎调用的句柄 long		
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual long HSDataSource_DataSouceInit(IN IDataSourceSink* pSink, IN const char* pszProfile = NULL) = 0;


	/********************************************************************************
	* 函数功能:数据引擎注销
	* 函数参数:IDataSourceSink* pSink - 回调的句柄
	*			  
	*  返回值 :TRUE成功 
	*  		
	* 调用依赖:	HSDataSource_DataSouceInit
	* 作   者:
	* 完成日期:
	* 修改记录:
	*******************************************************************************/
	virtual BOOL HSDataSource_DataSourceUnInit(IN long lDataSourceID) = 0;


	/********************************************************************************
	* 函数功能:异步数据请求，请求发送后，立即返回，请求结果将在回调中给出
	* 函数参数:long lDataSourceID - HSDataSource_SetCommInit的返回结果
	*			
	*			char* pAskData - 请求包内容 ---- 当请求包为空，长度为0时，认为是从lDataSourceID 需要一个清空包，取消所有订阅
	*			int nAskLen - 请求包大小
	*			int nTimeOut - 超时时间
	*			UINT uSendPriority = e_DataSouceSend_Normal - 数据发送选项，包括优先级、清空包等
	*			BOOL bReadDisk              是否读缓存
	*			long lConnectHandle = 0 - 连接句柄
	*			long lPackID - 包序号
	*  返回值 :TRUE 成功	
	* 调用依赖: HSDataSource_SetCommInit	
	* 作   者:
	* 完成日期:
	* 修改记录: 
	*******************************************************************************/

	virtual BOOL HSDataSource_RequestAsyncData(IN long lDataSourceID, IN char* pAskData, IN int nAskLen, IN int nTimeOut, 
		IN UINT uSendOptions = e_DataSouceSend_Normal, IN long lConnectHandle = 0, IN long lPackID = 0) = 0;

	/********************************************************************************
	* 函数功能:同步数据请求，数据应答后再返回结果
	* 函数参数:long lDataSourceID - HSDataSource_SetCommInit的返回值
	*			
	*			char* pAskData - 请求包内容
	*			int nAskLen - 请求包大小
	*			int nTimeOut - 超时时间
	*			int& pLen - 返回包大小
	*			UINT uSendPriority = e_DataSouceSend_Normal - 数据发送选项，包括优先级、清空包等
	*			long lConnectHandle = 0 - 连接句柄
	*			long lPackID - 包序号
	*  返回值 :	数据包指针	
	* 调用依赖: HSDataSource_SetCommInit	
	* 作   者:
	* 完成日期:
	* 修改记录:
	*******************************************************************************/
	virtual void* HSDataSource_RequestSyncData(IN long lDataSourceID, IN char* pAskData, IN int nAskLen, IN int nTimeOut, 
		IN OUT int& pLen, IN UINT uSendOptions = e_DataSouceSend_Normal, IN long lConnectHandle = 0, IN long lPackID = 0) = 0;

	/********************************************************************************
	* 函数功能:同步数据处理后，数据内存的引用删除
	* 函数参数:long lDataSourceID - HSDataSource_SetCommInit的返回值
	*			
	*			void* pRefData - 同步数据返回的结果指针
	*			int nRefDataLen - 数据包大小
	*			long lConnectHandle - 连接句柄
	*  返回值 :TRUE 成功	
	* 调用依赖: HSDataSource_RequestSyncData	
	* 作   者:
	* 完成日期:
	* 修改记录:
	*******************************************************************************/
	virtual BOOL HSDataSource_SyncDataRefComplete(IN long lDataSourceID, IN void* pRefData, IN int nRefDataLen, IN long lConnectHandle = 0) = 0;

	/********************************************************************************
	* 函数功能:获取最后错误内容
	* 函数参数:long lConnectHandle - 连接句柄
	*			
	*			
	*			
	*			
	*  返回值 :错误内容	
	* 调用依赖: HSDataSource_RequestSyncData	
	* 作   者:
	* 完成日期:
	* 修改记录:
	*******************************************************************************/
	virtual CString	HSDataSource_GetLastError(IN long lConnectHandle = 0) = 0;

	/********************************************************************************
	* 函数功能:登录或外部其他模块调用数据引擎，做数据初始化
	* 函数参数:
	*			
	*			
	*			
	*			
	*  返回值 : 0 成功
	* 调用依赖: HSDataSource_RequestSyncData	
	* 作   者:
	* 完成日期:
	* 修改记录:
	*******************************************************************************/
	virtual long	HSDataSource_ServerDataInit(IN long lConnectHandle, IN PFN_INITSTATUS_NOTIFY fnnotify, IN CWnd* pWnd) = 0;
	virtual void    HSDataSource_SetDefaultConnectHandle(IN long lConnectHandle) = 0;
	/********************************************************************************
	* 函数功能:登录或外部其他模块调用数据引擎，重连通知
	* 函数参数:
	*			
	*			
	*			
	*			
	*  返回值 : 0 成功
	* 调用依赖: HSDataSource_ReConnectNotify	
	* 作   者:
	* 完成日期:
	* 修改记录:
	*******************************************************************************/
	virtual BOOL HSDataSource_ReConnectNotify(IN long lConnectHandle) = 0;
};
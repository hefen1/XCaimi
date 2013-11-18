/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	DataSourceDefine.h
*	文件标识：
*	摘	  要：	数据引擎回调接口定义头文件
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-7-14
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma  once

#include "NetEngineInterface.h"

enum CommNotifyType  //状态通知类型
{
	eDataSource_ReConnect = 1,
	eDataSource_Init = 2
};

struct CommNotify
{
	CommNotifyType       m_uType; 
	Server_type               m_svrType;
	char				 m_strMsg[256];
};

class IDataSourceSink
{
public:
	/********************************************************************************
	* 函数功能 :数据引擎通讯回调
	* 函数参数 :void* pData - 指向一个CNI_General结构的指针，里面带有通讯状态
	*  返回值  :TRUE 成功		
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual BOOL HSDataSourceSink_OnCommNotify(void* pData) = 0;

	/********************************************************************************
	* 函数功能 :数据引擎数据回调 (针对交易模块，该过程将做解包过程)
	* 函数参数 :void* pData - 指向一个CNI_ReceiveData结构的指针
	*			int nLen - 数据长度
	*  返回值  :TRUE 成功		
	* 调用依赖 :	
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual BOOL HSDataSourceSink_OnRecvData(void* pData, int nLen) = 0;

};

class IDataSourceSinkEx : public IDataSourceSink
{
public:
	/********************************************************************************
	* 函数功能 :数据引擎调用交易模块 同步到主线程处理
	* 函数参数 :
	*			
	*  返回值  :	
	* 调用依赖 :	HSDataSourceSink_OnRecvData
	* 作    者 :
	* 完成日期 :
	* 修改记录 :
	*******************************************************************************/
	virtual BOOL HSDataSourceSinkEx_RecvDataNotify() = 0;
};
/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	AutoRequest.h
*	文件标识：
*	摘	  要：	数据引擎中的自动请求类
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
#include "NetEngineInterface.h"

#pragma once

class CDataSourceImpl;
class CConfigManager;

#define TEMP_DATASOURCE_ID						1					//暂时使用，本地发初始化标志

enum	eDataSourceAutoInitStatus			//数据引擎中自动初始化状态
{
	InitStatus_succ = 0,			//初始化完成
	InitStatus_login = 1,			//登录
	InitStatus_config = 2,			//下载配置
	InitStatus_exright = 3,			//除权数据
	InitStatus_currentfinancedata = 4, //财务数据
	InitStatus_blockdata = 5,		//板块数据
	InitStatus_userblockdata = 6,	//用户板块
	InitStatus_init = 7,			//初始化市场、股票
	InitStatus_servercalc = 8,		//计算涨跌停
};

enum eDataSourceAutoConfigType
{
	ConfigType_bystatus = 1,
	ConfigType_byRequest = 2,
};

class CAutoRequest
{
protected:
	CDataSourceImpl*		m_pDataSourceImpl;
	CConfigManager*			m_pConfigManager;

	CConfigManager*			GetConfigManager();
public:
	//	构造函数与析构函数
	CAutoRequest();
	~CAutoRequest();

public:
	//设置数据引擎对象指针
	void				SetDataSourceImpl(CDataSourceImpl* pImpl);
	//登录成功的响应
	BOOL				OnLoginSuccess(Server_type svrType);
	//初始化请求
	BOOL				ReqServerInitInfo(Server_type svrType);
	//文件请求
	BOOL				RequestConfFile(CString strFileName, BOOL bForceUpdate = FALSE);
	// 请求增量财务数据与除权数据
	BOOL				RequestIncrementData(long lType);
	//转译状态文字
	CString				GetStatus(DWORD dwStatus);
	//转译成配置文件节点名字
	CString				GetSectionName(int nType, DWORD dwStatus);
	//请求股票（或其他）涨跌停数据
	BOOL				AutoRequestServerCalc(char cSvrType);
	//请求服务器信息
	BOOL				ReqServerInfo();		
};
/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：LoginInterface.h
* 摘    要：登录接口声明文件
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-10
* 备	 注：
*******************************************************************************/
#pragma once

#define OBJ_LOGIN	      _T("Login.LoginObj")		

enum LoginType{LoginType_Normal = 1, LoginType_OnlyTrade};
enum LoginRet {LoginRet_Error = 1, LoginRet_Quote, LoginRet_Trade, LoginRet_QuoteAndTrade};
interface IDoLogin
{
	virtual LoginRet Login(LoginType eType) = 0;  // 登录函数
	virtual void TestSpeed() = 0;                  // 测速
	virtual void ConnManager() = 0;                // 通讯设置
};
